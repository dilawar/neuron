/***
 *    Description:  Network.
 *
 *         Author:  Dilawar Singh <dilawars@ncbs.res.in>
 *   Organization:  NCBS Bangalore
 *        License:  MIT License
 */


#include "../include/Network.h"
#include "../include/SynapseGroup.h"
#include "../include/NeuronGroup.h"

#include "Connectors.hh"

// Global 
sc_time globalDt_;

Network::Network(sc_module_name name, double dt): 
    name_(name)
    , dt_(sc_time(dt, SC_SEC))
{ 

#ifdef LOG_LEVEL
    spdlog::set_level(spdlog::level::LOG_LEVEL);
#endif

    globalDt_ = dt_;

    path_ = string((const char*)name_);
    spdlog::info("Creating network: {}", path_ );

    // setup clock.
    clock_ = make_unique<sc_clock>("clock", dt_, SC_SEC);
    spdlog::info("Global clock dt {} s", clock_->period().to_seconds() );

    SC_METHOD(record);
    sensitive << clock_->posedge_event();
}

Network::~Network()
{
}

void Network::record()
{
    std::cout << sc_time_stamp().to_seconds() << " ";
    for (auto& s : signals_)
        cout << s.second->read() << ' ';
    for (auto& s : boolSignals_)
        cout << s.second->read() << ' ';
    cout << endl;

}

void Network::before_end_of_elaboration()
{
    //
}

// Add synapse groups.
void Network::addSynapseGroup(const string& path, size_t N
        , double gbar, double tau, double Esyn
        , const string type)
{
    SynapseGroup* syn = new SynapseGroup(path.c_str(), N, gbar, tau, Esyn, type);
    addToMaps<SynapseGroup>("SynapseGroup",  syn);
    spdlog::info("Created SynapseGroup: {} of size {}", path, N);

    // bind ports.
    string sigName;
    for (size_t i = 0; i < syn->size(); i++) 
    {
        SynapseBase* s = syn->getSynapse(i);

        // Connect spike.
        sigName = string(s->spike.name());
        auto spkSig = make_unique<sc_signal<bool>>();
        s->spike.bind(*spkSig);
        addBoolSignal(sigName, std::move(spkSig));

        // Connect post 
        sigName = string(s->post.name());
        auto postSig = make_unique<sc_signal<double>>();
        s->post.bind(*postSig);
        addSignal(sigName, std::move(postSig));

        // Connect psc 
        sigName = string(s->psc.name());
        auto pscSig = make_unique<sc_signal<double>>();
        s->psc.bind(*pscSig);
        addSignal(sigName, std::move(pscSig));

        // bind clock.
        s->clock(*clock_);
    }
}

// Neuron group.
void Network::addNeuronGroup(const string& path, size_t N, double rm, double cm, double Em)
{

    NeuronGroup* ng = new NeuronGroup(path.c_str(), N, rm, cm, Em);
    addToMaps<NeuronGroup>("NeuronGroup", ng);

    spdlog::info("Created NeuronGroup: {} of size {}. rm={}, cm={}, Em={}"
            , path, N, rm, cm, Em);

   // Now bind ports.
    string sigName;
    for (size_t i = 0; i < N; i++) 
    {
        IAF* n = ng->getNeuron(i);
        n->clock(*clock_);

        // Connect vm.
        sigName = string(n->vm.name());
        auto vmSig = make_unique<sc_signal<double>>();
        n->vm.bind(*vmSig);
        addSignal(sigName, std::move(vmSig));

        // Connect inject 
        sigName = string(n->inject.name());
        auto injectSig = make_unique<sc_signal<double>>();
        n->inject.bind(*injectSig);
        addSignal(sigName, std::move(injectSig));

    }
    spdlog::info("Created NeuronGroup: {} of size {}", path, N);
}

void Network::addSignal(const string& name, unique_ptr<sc_signal<double>> sig)
{
    signals_.insert( {name, std::move(sig)} );
}

void Network::addBoolSignal(const string& name, unique_ptr<sc_signal<bool>> sig)
{
    boolSignals_.insert( {name, std::move(sig) } );
}


// Add spike generator, Poisson Group.
void Network::addPoissonGroup(const string& path, size_t N, double lambda)
{
    spdlog::error( "Network::addSpikeGeneratorGroup is not implemented yet." );
    throw NotImplemented();
}

// Add a spikegenerator group.
void Network::addSpikeGeneratorGroup(const string& path, size_t N, const string type, ...)
{
    spdlog::error( "Network::addSpikeGeneratorGroup is not implemented yet." );
    throw NotImplemented();
}

void Network::addSpikeGeneratorPeriodicGroup(const string& path, size_t N, double period, double delay)
{
    SpikeGeneratorBase* spikeGen_ = new PeriodicSpikeGenerator(path.c_str(), N, period);
    spikeGen_->setDelay(delay);

    addToMaps<SpikeGeneratorBase>("SpikeGeneratorPeriodicGroup", spikeGen_);
    spdlog::info("Created SpikeGeneratorGroup {} with size {}", path, N);

    // Bind ports.
    bindPortSpikeGeneratorBase(spikeGen_);
}

void Network::bindPortSpikeGeneratorBase(SpikeGeneratorBase* spk)
{
    // DO NOT BIND CLOCK. Stimulus generator works in SC_THREAD.
    spk->clock(*clock_);

    for (size_t i = 0; i < spk->size(); i++) 
    {
        string sigName = (boost::format("%1%[%2%]")%spk->path()%i).str();
        auto sig = make_unique<sc_signal<bool>>(sigName.c_str(), false);
        spk->getSpikePort(i)->bind(*sig);
        addBoolSignal(sigName, std::move(sig));
    }
}

int Network::bindPorts(network_variant_t elem)
{
    spdlog::info( "Connecting ports. " );
    return boost::apply_visitor(std::bind(NetworkPortBinderVisitor(), std::placeholders::_1, this), elem);
}



string Network::path() const
{
    return path_;
}

network_variant_t Network::findGroup(const string& path)
{
    return elemMap_.find(path)->second; 
}

// Find by type.
void Network::findElementsByType(const string& type, std::vector<network_variant_t>& collect)
{
    auto range = typeMap_.equal_range(type); 
    for( auto i = range.first; i != range.second; i++)
        collect.push_back(i->second);
}

// Connect port.
int Network::connect(const string& srcPath, const string& srcPort
        , const string& tgtPath , const string& tgtPort)
{
    spdlog::info("Connecting {}.{} --> {}.{} ...", srcPath, srcPort, tgtPath, tgtPort);
    auto src = findGroup(srcPath);
    auto tgt = findGroup(tgtPath);
    return boost::apply_visitor(
            std::bind(NetworkConnectionVisitor(), std::placeholders::_1, srcPort, tgt, tgtPort)
            , src);
}
 



/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  sc_main/main function. This function is called when user simulate
 * system.
 *
 * @Param runtime
 *
 * @Returns   
 */
/* ----------------------------------------------------------------------------*/
int Network::start(double runtime)
{
    // Turn them to US.
    sc_start(runtime, SC_SEC);
#if 0
    try
    {
        sc_start(runtime, SC_SEC);
    } 
    catch(std::exception& e )
    {

        cerr << "Failed to run." << endl;
        cerr << e.what() << endl;
        
        throw e;
    }
#endif 

    return 0;
}


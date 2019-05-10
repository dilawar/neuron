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

Network::Network(sc_module_name name, double dt): 
    name_(name)
    , dt_(dt)
{ 

#ifdef LOG_LEVEL
    spdlog::set_level(spdlog::level::LOG_LEVEL);
#endif

    path_ = string((const char*)name_);
    spdlog::info("Creating network: {}", path_ );
    SC_METHOD(record);
    sensitive << clock;
}

Network::~Network()
{
}

void Network::record()
{
    std::cout << "Ticking " << std::endl;
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
        binarySignals_.insert({sigName,std::move(spkSig)});

        // Connect post 
        sigName = string(s->post.name());
        auto postSig = make_unique<sc_signal<double>>();
        s->post.bind(*postSig);
        signals_.insert({sigName, std::move(postSig)});

        // Connect psc 
        sigName = string(s->psc.name());
        auto pscSig = make_unique<sc_signal<double>>();
        s->psc.bind(*pscSig);
        signals_.insert({sigName, std::move(pscSig)});

        // bind clock.
        s->clock(clock);
    }
}

// Neuron group.
void Network::addNeuronGroup(const string& path, size_t N, double rm, double cm, double Em)
{
    NeuronGroup* ng = new NeuronGroup(path.c_str(), N, rm, cm, Em);

    // Now bind ports.
    string sigName;
    for (size_t i = 0; i < N; i++) 
    {
        IAF* n = ng->getNeuron(i);
        n->clock(clock);

        // Connect vm.
        sigName = string(n->vm.name());
        auto vmSig = make_unique<sc_signal<double>>();
        n->vm.bind(*vmSig);
        signals_.insert({sigName,std::move(vmSig)});

        // Connect inject 
        sigName = string(n->inject.name());
        auto injectSig = make_unique<sc_signal<double>>();
        n->inject.bind(*injectSig);
        signals_.insert({sigName, std::move(injectSig)});

    }
    spdlog::info("Created NeuronGroup: {} of size {}", path, N);
    // addToMaps<NeuronGroup>("NeuronGroup", ng);
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
    SpikeGeneratorBase* spikeGen_ = new PeriodicSpikeGenerator(path.c_str(), N);
    spikeGen_->clock(clock); // Bind clock.

    addToMaps<SpikeGeneratorBase>("SpikeGeneratorPeriodicGroup", spikeGen_);
    spdlog::info("Created SpikeGeneratorGroup {} with size {}", path, N);
}

int Network::bindPorts(void)
{
    spdlog::info( "Connecting ports. " );
    for(auto v: elemMap_)
        boost::apply_visitor(std::bind(NetworkPortBinderVisitor(), std::placeholders::_1, this)
                , v.second);
    return 0;
}



string Network::path() const
{
    return path_;
}

network_variant_t Network::findElementByPath(const string& path)
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
    bindPorts();

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


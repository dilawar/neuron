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
    spdlog::debug( "Bind rest of the ports here.");

    // List ports which are not bound.
    // spdlog::info( "Dumping hierarchy of module." );
    // cout << printHeir(this);
}

// Add synapse groups.
void Network::addSynapseGroup(const string& path, size_t N
        , double gbar, double tau, double Esyn
        , const string type)
{
    SynapseGroup* syn = new SynapseGroup(path.c_str(), N, gbar, tau, Esyn, type);
    addToMaps<SynapseGroup>("SynapseGroup",  syn);
    spdlog::info("Created SynapseGroup: {} of size {}", path, N);

    // Now bind ports.
    for (size_t i = 0; i < syn->size(); i++) 
    {
        auto * s = syn->getSynapse(i);
        string sigName(s->spike.name());
        auto sig = make_unique<sc_signal<double>>(sigName.c_str(), 0.0);
        // s->spike.bind(*sig);
        // signals_.insert({sigName,std::move(sig)});
    }
}

// Neuron group.
void Network::addNeuronGroup(const string& path, size_t N, double rm, double cm, double Em)
{
    NeuronGroup* ng = new NeuronGroup(path.c_str(), N, rm, cm, Em);

    ng->clock(clock);

    addToMaps<NeuronGroup>("NeuronGroup", ng);
    spdlog::info("Created SynapseGroup: {} of size {}", path, N);
}


// Add spike generator, Poisson Group.
void Network::addPoissonGroup(const string& path, size_t N, double lambda)
{
}

// Add a spikegenerator group.
void Network::addSpikeGeneratorGroup(const string& path, size_t N, const string type, ...)
{
}

void Network::addSpikeGeneratorPeriodicGroup(const string& path, size_t N, double period, double delay)
{
    SpikeGeneratorBase* spikeGen_ = new PeriodicSpikeGenerator(path.c_str(), N);
    spikeGen_->clock(clock); // Bind clock.
    addToMaps<SpikeGeneratorBase>("SpikeGeneratorPeriodicGroup", spikeGen_);

    spdlog::info("Created SpikeGeneratorGroup {} with size {}", path, N);
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

int Network::connect(const string& srcPath, const string& srcPort
        , const string& tgtPath , const string& tgtPort)
{

    spdlog::info("Connecting {}.{} --> {}.{} ...", srcPath, srcPort, tgtPath, tgtPort);

    auto src = findElementByPath(srcPath);
    auto tgt = findElementByPath(tgtPath);
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


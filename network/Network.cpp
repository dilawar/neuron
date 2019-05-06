/***
 *    Description:  Network.
 *
 *         Author:  Dilawar Singh <dilawars@ncbs.res.in>
 *   Organization:  NCBS Bangalore
 *        License:  MIT License
 */

#include "../include/Synapse.h"
#include "../include/Network.h"
#include "../external/spdlog/spdlog.h"

Network::Network(sc_module_name name): 
    name_(name)
    , dt_(1e-4)
{ 
    path_ = string((const char*)name_);
    spdlog::info("Creating network: {}", path_ );
    SC_METHOD(record);
    sensitive << clk_;
}

void Network::record()
{
    std::cout << "Ticking " << std::endl;
}

// Alpha synapses.
void Network::addSynapse(const string path, const string type)
{
    Synapse* syn = new Synapse(path.c_str(), type);

    // We need input spike train and output vm.
    syn->get()->clock(clk_);

    elements_["synapse.alpha"].push_back(boost::any(syn));
}

string Network::path() const
{
    return path_;
}

vector<boost::any> Network::getSynapses( )
{
    return elements_["synapse.alpha"];
}

vector<boost::any> Network::getSynapses(const string ctype)
{
    return elements_["synapse." + ctype];
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


void Network::SynapseGroup(size_t N
        , double gbar, double tau, double Esyn
        , const string type)
{
    for (size_t i = 0; i < N; i++) 
    {
        string synapseName = path() + "#synapse#" + to_string(i);
        spdlog::info("Creating synapse: {}", synapseName);
        synapses_.push_back(make_unique<Synapse>( synapseName, gbar, tau, Esyn, type));
    }
}


// Spike generation.
void Network::PoissonGroup(size_t N, double lambda)
{

}

/***
 *    Description:  Network.
 *
 *        Created:  2019-05-05

 *         Author:  Dilawar Singh <dilawars@ncbs.res.in>
 *   Organization:  NCBS Bangalore
 *        License:  MIT License
 */

#ifndef NETWORK_H
#define NETWORK_H

#include <systemc>
#include <memory>
#include <vector>
#include <map>
#include <boost/variant.hpp>

#include "../include/global.h"
#include "../include/SpikeGenerator.h"
#include "../include/SynapseGroup.h"
#include "../include/NeuronGroup.h"
#include "../utility/str_util.hpp"

using namespace std;

class Network : public sc_module 
{
    SC_HAS_PROCESS(Network);

public:

    Network(sc_module_name path, double dt);
    ~Network( );

    // Groups 
    void addSynapseGroup(const string& path, size_t N
            , double gbar, double tau, double Esyn
            , const string type="alpha"
            );

    void addNeuronGroup(const string& path, size_t N
            , double rm = 100e3, double cm = 100e-12, double Em = -65e-3
            );

    // Spike generation.
    void addPoissonGroup(const string& path, size_t N, double lambda);
    void addSpikeGeneratorGroup(const string& path, size_t N, const string type ...);
    void addSpikeGeneratorPeriodicGroup(const string& path, size_t N, double period, double delay=0);
    void addSpikeGeneratorPoissonGroup(const string& path, size_t N, double lambda);

    string path() const;
    void before_end_of_elaboration();

    // Monitor given target.
    int monitor(const string& tgt, const string& port);

    template<typename T>
    void addToMaps(const string type, T* const a)
    {
        assert( ! type.empty() );
        assert( a );
        assert( ! a->path().empty() );

        network_variant_t ba = a;
        typeMap_.insert({type, ba});
        elemMap_.insert( {a->path(), ba} );
    }

    // It uses boost::variant 
    int bindPorts( );

    // ACCESSORS.
    network_variant_t findElementByPath(const string& group);
    void findElementsByType(const string& type, std::vector<network_variant_t>& collect);

    // 
    void record();
    void gen_clock();

    // TODO: return error code 
    int start(double runtime);

private:
    sc_module_name name_;
    string path_;
    double dt_;                     // Timeperiod of clock

    // map of Type and vector of elements with those types. Store BaseClass as
    // type info.

    // multiple elements with same type.
    multimap<string, network_variant_t> typeMap_;

    // One element for a given path. Each element is of GroupType.
    map<string, network_variant_t> elemMap_;

    // All the ports which are out port should be connected to these signals.
    vector<sc_signal<bool>> records_;

    // Create map of signal to bind.
    map<string, unique_ptr<sc_signal<double>> > signals_;
    map<string, unique_ptr<sc_signal<bool>> > binarySignals_;

public:
    sc_clock clock{ "clock", 0.1, SC_MS };
    sc_signal<bool> event_;

};



#endif /* end of include guard: NETWORK_H */

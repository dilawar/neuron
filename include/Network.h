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
#include <type_traits>
#include <exception>
#include <vector>
#include <map>
#include <boost/variant.hpp>

#include "../utility/str_util.hpp"
#include "../utility/sc_utils.hpp"

#include "../include/global.h"
#include "../include/SpikeGenerator.h"
#include "../include/SynapseGroup.h"
#include "../include/NeuronGroup.h"

using namespace std;

class Network : public sc_module 
{
    SC_HAS_PROCESS(Network);

public:

    Network(sc_module_name path, double dt=1e-6);

    // Groups 
    void addSynapseGroup(const string& path, size_t N
            , double gbar, double tau, double Esyn
            , const string type="alpha"
            );

    void addNeuronGroup(const string& path, size_t N
            , double rm = 100e6       // 100 mega-ohm usually.
            , double cm = 100e-12     // 100 pF 
            , double Em = -65e-3      // 65 mV.
            );

    // Spike generation.
    void addPoissonGroup(const string& path, size_t N, double lambda);
    void addSpikeGeneratorGroup(const string& path, size_t N, const string type ...);
    void addSpikeGeneratorPeriodicGroup(const string& path, size_t N, double period, double delay=0);
    void addSpikeGeneratorPoissonGroup(const string& path, size_t N, double lambda);

    // fixme: Templated function? Had lot of issue with inserting into maps.
    void addSignal(unique_ptr<sc_signal<bool>> sig);
    void addSignal(unique_ptr<sc_signal<double>> sig);

    // Port binding.
    void bindPortSpikeGeneratorBase(SpikeGeneratorBase* ptr);
    void before_end_of_elaboration();

    // Monitor given target.
    int monitor(const string& tgt, const string& port);

    // Connect and bind.
    // It uses boost::variant 
    int connect(const string& a, const string& aport, const string& b, const string& bport);
    void bindUnboundPorts(void);
    int bindPorts(network_variant_t elem);

    // Getter.
    network_variant_t findGroup(const string& group);
    void findElementsByType(const string& type, std::vector<network_variant_t>& collect);
    string path() const;

    // Functions.
    void record();
    void gen_clock();
    void saveData(const string& which="", const string& sep=" ");

    // TODO: return error code 
    int start(double runtime);

    // Templated functions.
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



    // connect port.
    template<typename T=double, typename PortType=sc_core::sc_in<T> >
    void connectPort(PortType* port)
    {
        if(port->bind_count() == 0)
        {
            string sigName(port->name());
            sigName = sanitizePath(sigName);
            unique_ptr<sc_signal<T>> sig = make_unique<sc_signal<T>>(sigName.c_str(), (T)0);
            port->bind(*sig);
            addSignal(std::move(sig));
            spdlog::debug( "\t+ Connected unbound port {}", port->name());
        }
    }

private:
    sc_module_name name_;
    string path_;
    sc_time dt_;                     // Timeperiod of clock

    // map of Type and vector of elements with those types. Store BaseClass as
    // type info.

    // multiple elements with same type.
    multimap<string, network_variant_t> typeMap_;

    // One element for a given path. Each element is of GroupType.
    map<string, network_variant_t> elemMap_;

    // Create map of signal to bind.
    map<string, unique_ptr<sc_signal<double>> > signals_;
    map<string, unique_ptr<sc_signal<bool>> > boolSignals_;

    // Store data.
    map<string, vector<double>> data_;

public:
    // sc_clock clock{ "clock", 0.1, SC_MS };
    unique_ptr<sc_clock> clock_;
    sc_signal<bool> event_;

};



#endif /* end of include guard: NETWORK_H */

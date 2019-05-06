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

#include <memory>
#include <vector>
#include <map>
#include <systemc>
#include <boost/any.hpp>

class Synapse;

using namespace std;

class Network : public sc_module 
{
    SC_HAS_PROCESS(Network);

public:

    Network(sc_module_name path);

    void addSynapse(const string path, const string = "alpha");
    void addSynapse(const Synapse* ptr);

    void addNeuron(const string path, const string& type="iaf");

    // Groups 
    void SynapseGroup(size_t N, 
            double gbar, double tau, double Esyn
            , const string type="alpha"
            );

    void NeuronGroup(size_t N);

    // Spike generation.
    void PoissonGroup(size_t N);
    void SpikeGenerator(size_t N);

    vector<boost::any> getSynapses( );
    vector<boost::any> getSynapses(const string ctype);

    string path() const;

    // 
    void record();
    void gen_clock();

    // TODO: return error code 
    int start(double runtime);

private:
    sc_module_name name_;
    string path_;
    double dt_;                     // Timeperiod of clock

    /* data */
    vector<unique_ptr<Synapse> > synapses_;
    map<string, vector<boost::any> > elements_;

public:
    sc_clock clk_{ "clock", 0.1, SC_MS };
    sc_signal<bool> event_;

};



#endif /* end of include guard: NETWORK_H */

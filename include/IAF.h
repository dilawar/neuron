// =====================================================================================
//
//       Filename:  IAF.h
//
//    Description:  Integrate and fire neuron.
//
//        Version:  1.0
//        Created:  Saturday 27 April 2019 04:52:03  IST
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Dilawar Singh (), dilawar.s.rajput@gmail.com
//   Organization:  NCBS Bangalore
//
// =====================================================================================

#ifndef IAF_H
#define IAF_H

#define MAX_SYNAPSES 100

#include <systemc.h>
#include <vector>
#include <tuple>
#include <array>
#include <memory>
#include <random>

#include "SynapseExp.h"
#include "SynapseAlpha.h"
#include "../include/sc_ports.hpp"

using namespace std;

class IAF : public sc_module
{
public:
    SC_HAS_PROCESS(IAF);


    //-----------------------------------------------------------------------------
    //  Constructors.
    //-----------------------------------------------------------------------------
    IAF(sc_module_name path, double em=-65e-3, double tau=10e-3);
    IAF(sc_module_name path, double em, double cm, double rm);

    void init();

    //-----------------------------------------------------------------------------
    //  Ports
    //-----------------------------------------------------------------------------
    sc_in_clk clock{"clock"};                       // Incoming clock.

    sc_out_optional<double> vm{"vm"};               // Neuron membrane potential.
    sc_in_optional<double> inject{"inject"};        // Injection of current by users.

    sc_signal<double> synapse_inject[MAX_SYNAPSES]; // Currents from all synapses.

    //-----------------------------------------------------------------------------
    //  Events.
    //-----------------------------------------------------------------------------
    sc_event nonZeroInject;                 // If non-zero injection of current.
    sc_event nonZeroSynCurrent;             // If non-zero injection via synapses.
    sc_event onFire;                        // If this neuron has fired.

    void decay();
    void record();
    void handleInjection();                 // Tick when there is inject
    void handleSynapticInjection();
    void handleOnFire();

    void model(const double &vm, double &dvdt, const double t);

    //-----------------------------------------------------------------------------
    //  Mutators.
    //-----------------------------------------------------------------------------
    void addSynapse(shared_ptr<SynapseBase> syn);
    void setRefactory(double t);
    void setTau(double t);
    void setNoise(double eps);
    void setThreshold(double t);

    //-----------------------------------------------------------------------------
    //  Accessor.
    //-----------------------------------------------------------------------------
    std::vector<std::tuple<double, double>> data( ) const;
    string path() const;

    //-----------------------------------------------------------------------------
    //  Helper
    //-----------------------------------------------------------------------------
    string repr();
    double noise();

    void save_data(const string& outfile="");

private:
    // Collect synapses.
    string path_;

    std::vector<std::tuple<double, double>> data_;
    std::vector<double> spikes_;            // Time of spikes.
    vector<shared_ptr<SynapseBase>> synapses_;

    double Cm_;
    double Em_;
    double Rm_;
    double vm_;
    double tau_;
    double threshold_;
    double refactory_;
    bool fired_;

    // Helper variables.
    double t_, prevT_;
    double dt_;
    double sum_all_synapse_inject_;

    // noise source.
    std::default_random_engine gen_;
    std::uniform_real_distribution<double>  dist_{-1, 1};
    double noise_;
};

#endif /* end of include guard: IAF_H */

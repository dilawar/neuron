/*
 *  Chemical synapse modelled by alpha function.
 *
 *  A synapse is a three port device. 
 *     in       : double, volage of pre-synaptic side.
 *     out      : double, volage of post-synaptic side.
 *     inject   : double, current value which can be injected into post synaptic side.
 *
 *  Params:
 *      Esyn: Reversal potential of ion-channel that mediate the synaptic
 *      current.
 */

#ifndef EXPSYNAPSE_H
#define EXPSYNAPSE_H

#include "systemc.h"
#include <boost/units/systems/si.hpp>
#include <boost/units/systems/si/io.hpp>
#include <vector>

using namespace boost::units;
namespace si = boost::units::si;

struct ExpSynapse: public sc_module
{
    SC_HAS_PROCESS(ExpSynapse);
    sc_in_clk clock;

    // Can't force units here. Since it is based on SC_port. 
    // TODO: It can be done but may be later. See https://www.doulos.com/knowhow/systemc/faq/#q1 
    sc_in<double> pre;
    sc_in<double> post;
    sc_out<double> inject;

    void process() 
    {
        vPre_ = pre.read() * si::volt;
        vPost_ = post.read() * si::volt;
        cout << engineering_prefix << "vPre:" << vPre_ << " vPost:" << vPost_ << endl;

        inject.write( quantity_cast<double>(vPost_)/ 10.0 );
    }

    ExpSynapse(sc_module_name name, double Esyn = 0.0, double tau1=3e-3) : 
        name_(name)
        , Esyn_( Esyn*si::volt )
        , tau1_(sc_time(tau1, SC_SEC))
    {
        SC_METHOD(process);
        sensitive << clock.pos();
        g_ = 0.0*si::siemens;
    }

    sc_module_name name_;
    quantity<si::electric_potential> Esyn_;

    quantity<si::conductance> g_;
    quantity<si::electric_potential> vPre_, vPost_;
    quantity<si::current> inject_;

    sc_time tau1_, tau2_;
    std::vector<sc_time> spikeTimes;
};

#endif /* end of include guard: EXPSYNAPSE_H */

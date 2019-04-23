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

#ifndef SYNAPSEBASE_H
#define SYNAPSEBASE_H


#include "systemc.h"
#include <boost/units/systems/si.hpp>
#include <boost/units/systems/si/io.hpp>
#include <vector>

using namespace boost::units;
namespace si = boost::units::si;

/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  Synapse class. 
 *
 * This implements the most generic synapse. 
 * TODO: Variout constructors are provided to instantiate different types of synapss.
 */
/* ----------------------------------------------------------------------------*/
class Synapse: public sc_module
{

    public:
        SC_HAS_PROCESS(Synapse);
        sc_in_clk clock;

        // NOTE: Can't force units here. Since it is based on SC_port. 
        // TODO: It can be done but may be later. See https://www.doulos.com/knowhow/systemc/faq/#q1 

        /* Bool type. Incoming spike. */
        sc_in<bool> pre;  
        /* Read only (post synaptic potential) */
        sc_in<double> post;
        /* Amount of current injected into post synapse. */
        sc_out<double> inject; 


        /*-----------------------------------------------------------------------------
         *  At each tick, these process function computes the model.
         *-----------------------------------------------------------------------------*/
        void beforeProcess();                   /* Single exp synapse. */
        void processSingleExp();                /* Single exp synapse. */
        void processAlpha();                    /* Alpha synapse */
        void processTwoExp();                   /* Dual exponential synapse. */

        //-----------------------------------------------------------------------------
        //  Helper function.
        //-----------------------------------------------------------------------------
        void injectCurrent();

        // Synapse(sc_module_name name);
        Synapse(sc_module_name name);

        /* Alpha synapse */
        Synapse(sc_module_name name, double gbar, double tau, double Esynl, bool isalpha=true);
        /* Dual exp synapse */
        Synapse(sc_module_name name, double gbar, double tau1, double tau2, double Esyn);

        sc_module_name name_;
        quantity<si::conductance> g_, gbar_, leftover_;
        quantity<si::time> tau1_, tau2_;            /* Decay contants. */
        quantity<si::electric_potential> Esyn_;
        quantity<si::electric_potential> vPre_, vPost_;

        quantity<si::time> t_;               /* Current Time. */
        quantity<si::time> ts_;                     /* Previous firing. */
        
        // Keep the spike timings.
        std::vector<quantity<si::time>> t_spikes_;
};

#endif /* end of include guard: SYNAPSEBASE_H */

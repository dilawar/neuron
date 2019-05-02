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

#include <systemc.h>
#include <vector>
#include <tuple>
#include <array>
#include <memory>

#include "OdeSystem.h"

/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  Synapse class. 
 *
 * This implements the most generic synapse. 
 *
 * All units must be SI.
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
        sc_in<bool> spike;  

        /* Read only (post synaptic potential) */
        sc_in<double> post;

        /* Post synaptic current. */
        sc_out<double> inject; 


        /*-----------------------------------------------------------------------------
         *  At each tick, these process function computes the model.
         *-----------------------------------------------------------------------------*/
        void processAlpha();                    /* Alpha synapse */
        void processODE();                      /* Use odeint to solve. */
        void monitor_spike();                   

        void generateODEClock( );
        void start_of_simulation();


        //-----------------------------------------------------------------------------
        //  Generate clocks.
        //-----------------------------------------------------------------------------
        void tickOdeClock();

        //-----------------------------------------------------------------------------
        //  Helper function.
        //-----------------------------------------------------------------------------
        void injectCurrent();
        std::string repr();
        std::string name();
        void printODEData();

        void save_data(const std::string& filename="");

        // Synapse(sc_module_name name);
        Synapse(sc_module_name name);

        /* Alpha synapse */
        Synapse(sc_module_name name, double gbar, double tau, double Esynl, bool isalpha=true);

        /* Dual exp synapse (use ode solver) */
        Synapse(sc_module_name name, double gbar, double tau1, double tau2, double Esyn
                , double odedt=3e-5
                );

        const std::vector<std::tuple<double, double> >*  data() const;

    private:

        std::string name_;
        double g_, gbar_, leftover_;
        double tau1_, tau2_;            /* Decay contants. */
        double Esyn_;
        double vPre_, vPost_;

        double t_;                  /* Current Time. */
        double prevT_;              /* Previous tick time. ODE solver. */
        double tspike_;             /* Previous firing. */
        
        // Keep the spike timings.
        std::vector<double> t_spikes_;
        
        // Ode System
        std::unique_ptr<SynapseODESystem> odeSys_;

        sc_signal<bool> ode_clock;              // A slower clock of ODE solver.
        double ode_tick_;
        state_type state_;
        std::vector<std::tuple<double, double>> injectVec_;
        std::vector<std::tuple<double, double>>  data_;
};

#endif /* end of include guard: SYNAPSEBASE_H */

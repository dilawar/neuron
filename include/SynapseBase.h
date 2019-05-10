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


#include <vector>
#include <tuple>
#include <array>
#include <memory>
#include <systemc.h>
#include "../utility/sc_ports.hpp"

using namespace std;

/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  SynapseBase class.
 *
 * This implements the most generic synapse which inject current `inject` into
 * post-synaptic neuron:
 *      
 *      inject(t) = gbar(t) (V(t) - Esyn);
 *
 *  gbar(t) can have multiple forms which is updated in their respective
 *  classes.
 *
 * All units must be SI.
 */
/* ----------------------------------------------------------------------------*/
class SynapseBase: public sc_module
{

public:
    SC_HAS_PROCESS(SynapseBase);
    sc_in_clk clock{"clock"};

    SynapseBase(sc_module_name name, double gbar, double tau, double Esyn);
    ~SynapseBase();

    /* Bool type. Incoming spike. */
    sc_in<bool> spike{"spike"};

    /* Read only (post synaptic potential) */
    sc_in<double> post{"post"};

    /* Post synaptic current. */
    sc_out_opt<double> psc{"psc"};

    // Overridden in derived classes.
    virtual void process() = 0;

    //-----------------------------------------------------------------------------
    //  Helper functions.
    //-----------------------------------------------------------------------------
    void monitor_spike();
    void injectCurrent();
    void save_data(const std::string& filename="");
    
    //-----------------------------------------------------------------------------
    //  Setters.
    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    //  Getters
    //-----------------------------------------------------------------------------
    std::string repr();
    std::string path();

    const std::vector<std::tuple<double, double> >*  data() const;

protected:
    string path_;

    double g_, gbar_, leftover_;
    double tau1_, tau2_;            /* Decay contants. */
    double Esyn_;
    double vPre_, vPost_;

    double t_;                  /* Current Time. */
    double prevT_;              /* Previous tick time. ODE solver. */
    double tspike_;             /* Previous firing. */

    // Keep the spike timings.
    std::vector<double> t_spikes_;

    std::vector<std::tuple<double, double>> injectVec_;
    std::vector<std::tuple<double, double>>  data_;
};

#endif /* end of include guard: SYNAPSEBASE_H */

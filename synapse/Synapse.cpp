/*
 *    Description:  
 *
 *         Author:  Dilawar Singh (), dilawars@ncbs.res.in
 *   Organization:  NCBS Bangalore
 */

#include <memory>
#include <sstream>
#include <boost/format.hpp>
#include <boost/log/trivial.hpp>
#include <boost/numeric/odeint.hpp>

#include "Synapse.h"
#include "../engine/engine.h"

using namespace std;
using namespace boost;
namespace odeint = boost::numeric::odeint;

Synapse::Synapse(sc_module_name name) : name_(name)
{
    g_ = 0.0*si::siemens;
}

std::string Synapse::repr()
{
    std::stringstream ss;
    ss << boost::format("SYNAPSE:%1%, gbar=%2%, Esyn=%3% tau1=%4% tau2=%5%\n") 
        % name_ % gbar_ % Esyn_ % tau1_ % tau2_;
    return ss.str();
}


Synapse::Synapse(sc_module_name name, double gbar, double tau, double Esyn, bool isalpha): 
    name_(name) 
    , gbar_(gbar*si::siemens)
    , tau1_(tau*si::second)
    , Esyn_(Esyn*si::volt)
{
    SC_METHOD(processAlpha)
    sensitive << clock.neg();
    g_ = 0.0*si::siemens;
    BOOST_LOG_TRIVIAL(debug) << repr();
}

/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  This setup a synapse which is active only when a spike is
 * detected.
 *
 * @Param name
 * @Param gbar
 * @Param tau1
 * @Param tau2
 * @Param Esyn
 * @Param dt
 */
/* ----------------------------------------------------------------------------*/
Synapse::Synapse(sc_module_name name, double gbar, double tau1, double tau2
        , double Esyn, double odedt
        ): 
    name_(name) 
    , gbar_(gbar*si::siemens)
    , tau1_(tau1*si::second)
    , tau2_(tau2*si::second)
    , Esyn_(Esyn*si::volt)
    , ode_tick_(odedt)
{

    g_ = 0.0*si::siemens;
    state_[0] = g_/(1*si::siemens);
    state_[1] = g_/(1*si::siemens);
    odeSys_ = std::make_unique<SynapseODESystem>(gbar_, tau1_, tau2_);
    BOOST_LOG_TRIVIAL(debug) << repr();

    SC_THREAD( tickOdeClock );

    SC_METHOD(processODE);
    sensitive << ode_clock;
}

void Synapse::tickOdeClock(void)
{
    while(true)
    {
        ode_clock.write(! ode_clock.read());
        wait(ode_tick_, SC_SEC);
    }
}

/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  Helper function to do few operations before we compute the
 * conductance. 
 *
 * @Returns If spike occurs at this tick, return true, else false. Useful when
 * using ODE solver.
 */
/* ----------------------------------------------------------------------------*/
bool Synapse::beforeProcess( )
{
    bool spiked = false;

    t_ = sc_time_stamp().to_seconds() * si::second;
    vPost_ = post.read()*si::volt;

    // Time of previous spike.
    if(pre.read())
    {
        ts_ = t_;
        leftover_ = g_;
        t_spikes_.push_back(t_);
        spiked = true;
        if(odeSys_)
            odeSys_->addSpike(t_);
    }
    return spiked;
}

void Synapse::injectCurrent( )
{
    inject.write(quantity_cast<double>(g_*(vPost_-Esyn_)));
}

void Synapse::processAlpha() 
{
    beforeProcess();
    g_ = 0.0*si::siemens;
    for (auto tSpike : t_spikes_)
    {
        if(tSpike < t_)
        {
            auto T = (t_-tSpike)/tau1_;
            g_ += gbar_ * T * exp(-T);
        }
    }
    injectCurrent();
}

/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  Start of simulation.
 */
/* ----------------------------------------------------------------------------*/
void Synapse::start_of_simulation(void)
{
    // sc_clock *channel = dynamic_cast<sc_clock *>(clock.get_interface());
    // cout << (channel ? channel->period() : SC_ZERO_TIME).to_seconds()*si::second;
} 

/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  Solve the simple using ODE solver. 
 * To make computation faster, tick the solver when there is spike (positive edge).
 */
/* ----------------------------------------------------------------------------*/
void Synapse::processODE() 
{
    // Call to this function will put the spike in the vector.
    beforeProcess();

    // if(t_spikes_.size() < 1)
        // return;

    //// Now solve the ODE system.
    //double curT = t_spikes_[t_spikes_.size()-1]/si::second;
    //double lastT = 0.0;
    //if(t_spikes_.size() > 1)
    //    lastT = t_spikes_[t_spikes_.size()-2]/si::second;

    double curT = sc_time_stamp().to_seconds();
    if(curT < ode_tick_)
        return;
    double lastT = curT-ode_tick_;

    cout << name_ << ": " << curT << " " << lastT << ' ' << state_[0] << endl;

   odeint::integrate_adaptive(
            rk_dopri_stepper_type_()
            , [this](const state_type &dy, state_type &dydt, double t) {
                this->odeSys_->step(dy, dydt, t); 
            }, state_, lastT, curT, ode_tick_
            //, synapse_observer(data_)
            );
    g_ = state_[0]*si::siemens;
    injectCurrent();
}


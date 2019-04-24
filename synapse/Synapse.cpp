/*
 *    Description:  
 *
 *         Author:  Dilawar Singh (), dilawars@ncbs.res.in
 *   Organization:  NCBS Bangalore
 */

#include <boost/numeric/odeint.hpp>
#include <memory>

#include "Synapse.h"
#include "../engine/engine.h"



Synapse::Synapse(sc_module_name name) : name_(name)
{
    g_ = 0.0*si::siemens;
}


Synapse::Synapse(sc_module_name name, double gbar, double tau, double Esyn, bool isalpha): 
    name_(name) 
    , gbar_(gbar*si::siemens)
    , tau1_(tau*si::second)
    , Esyn_(Esyn*si::volt)
{
    if(isalpha)
        SC_METHOD(processAlpha)
    else
        SC_METHOD(processSingleExp);
    sensitive << clock.neg();
    g_ = 0.0*si::siemens;
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
Synapse::Synapse(sc_module_name name, double gbar, double tau1, double tau2, double Esyn, double odedt): 
    name_(name) 
    , gbar_(gbar*si::siemens)
    , tau1_(tau1*si::second)
    , tau2_(tau2*si::second)
    , Esyn_(Esyn*si::volt)
{
    SC_METHOD(processODE);
    sensitive << pre << ode_clock;

    // Generate slow clock.
    SC_THREAD(generateODEClock);

    g_ = 0.0*si::siemens;
    state_[0] = g_/(1*si::siemens);
    state_[1] = g_/(1*si::siemens);
    odeSys_ = std::make_unique<SynapseODESystem>(gbar_, tau1_, tau2_);
}

void Synapse::generateODEClock( )
{
    // Generate ODE clock.
    while(true)
    {
        ode_clock = ! ode_clock;
        wait(0.1, SC_MS);
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

void Synapse::processSingleExp() 
{
    beforeProcess();

    assert(tau1_ > 0.0*si::second);
    auto dt = (t_-ts_);
    double T = dt/tau1_;
    g_ = (gbar_ + leftover_) * exp(-T);
    injectCurrent();
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
 * @Synopsis  Solve the simple using ODE solver. To make computation faster,
 * tick the solver when there is spike.
 */
/* ----------------------------------------------------------------------------*/
void Synapse::processODE() 
{
    beforeProcess();
    static double last_tick_ = 0.0;
    // Now solve the ODE system.
    double curT = sc_time_stamp().to_seconds();
    if(curT == 0)
        return;

    // Check the results.
    // cout << "x " << state_[0] << ' ' << state_[1] << ' ' << last_tick_ << ' ' << curT << endl;
    boost::numeric::odeint::integrate_adaptive( rk_karp_stepper_type_()
            , [this](const state_type &dy, state_type &dydt, double t) {
                this->odeSys_->step(dy, dydt, t);
                }, state_, last_tick_, curT, std::min(1e-3,curT-last_tick_)
            );
    last_tick_ = curT;
    g_ = state_[0]*si::siemens;
    // cout << g_ << endl;
    injectCurrent();
}


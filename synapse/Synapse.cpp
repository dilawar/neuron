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

Synapse::Synapse(sc_module_name name, double gbar, double tau1, double tau2, double Esyn): 
    name_(name) 
    , gbar_(gbar*si::siemens)
    , tau1_(tau1*si::second)
    , tau2_(tau2*si::second)
    , Esyn_(Esyn*si::volt)
{
    SC_METHOD(processODE);
    sensitive << clock.neg();
    g_ = 0.0*si::siemens;

    state_[0] = g_/(1*si::siemens);
    state_[1] = g_/(1*si::siemens);
    odeSys_ = std::make_unique<SynapseODESystem>(gbar_, tau1_, tau2_);
}

void Synapse::beforeProcess( )
{
    t_ = sc_time_stamp().to_seconds() * si::second;
    vPost_ = post.read()*si::volt;

    // Time of previous spike.
    if(pre.read())
    {
        ts_ = t_;
        leftover_ = g_;
        t_spikes_.push_back(t_);

        if(odeSys_)
            odeSys_->addSpike(t_);
    }
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

void Synapse::processODE() 
{
    beforeProcess();
    // Now solve the ODE system.
    boost::numeric::odeint::runge_kutta4< state_type > stepper;
    double curT = sc_time_stamp().to_seconds();
    double prevT = std::max(0.0, curT-1e-3);

    // Check the results.
    // cout << "x " << state_[0] << ' ' << state_[1] << ' ' << curT << ' ' << prevT << endl;
    boost::numeric::odeint::integrate_const(stepper
            , [this](const state_type &dy, state_type &dydt, double t) {
                this->odeSys_->step(dy, dydt, t);
                }, state_, prevT, curT, 0.01
            );
}


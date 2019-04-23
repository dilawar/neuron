/*
 *    Description:  
 *
 *         Author:  Dilawar Singh (), dilawars@ncbs.res.in
 *   Organization:  NCBS Bangalore
 */

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
    SC_METHOD(processTwoExp);
    sensitive << clock.neg();
    g_ = 0.0*si::siemens;
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
    }
}

void Synapse::injectCurrent( )
{
    inject.write(quantity_cast<double>(g_*(vPost_-Esyn_)));
}

void Synapse::processSingleExp() 
{
    beforeProcess();

    // Now compute gsyn.
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

void Synapse::processTwoExp() 
{

    std::cout << "Not implemented yet." << std::endl;
}


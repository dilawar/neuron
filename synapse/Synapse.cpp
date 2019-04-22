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


Synapse::Synapse(sc_module_name name, double gbar, double tau, double Esyn): 
    name_(name) 
    , gbar_(gbar*si::siemens)
    , tau1_(tau*si::second)
    , Esyn_(Esyn*si::volt)
{
    SC_METHOD(processAlpha);
    sensitive << clock.pos();
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
    sensitive << clock.pos();
    g_ = 0.0*si::siemens;
}



void Synapse::processAlpha() 
{
    currTime_ = sc_time_stamp().to_seconds() * si::second;
    vPost_ = post.read()*si::volt;

    // Time of previous spike.
    if(pre.read() == true)
        ts_ = currTime_;

    if(ts_ == 0.0*si::second)
        return;

    // Now compute gsyn.
    auto dt = (currTime_ - ts_);
    assert(tau1_ > 0.0*si::second);

    g_ = gbar_ * tantrika::alpha(quantity_cast<double>(dt), quantity_cast<double>(tau1_));
    cout << '\t' << name_ << ": " << g_ << " " << gbar_ << " " << dt << " " << tau1_ << " " << endl;

    inject.write(quantity_cast<double>(g_*(vPost_-Esyn_)));
}

void Synapse::processTwoExp() 
{

    std::cout << "Not implemented yet." << std::endl;
}


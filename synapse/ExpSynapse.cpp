/*
 *       Filename:  ExpSynapse.c
 *
 *    Description:  
 *
 *         Author:  Dilawar Singh (), dilawars@ncbs.res.in
 *   Organization:  NCBS Bangalore
 */

#include "ExpSynapse.h"
#include "../engine/engine.h"


ExpSynapse::ExpSynapse(sc_module_name name, double gbar, double tau1, double Esyn): 
    name_(name) 
    , gbar_(gbar*si::siemens)
    , tau1_(tau1*si::second)
    , Esyn_(Esyn*si::volt)
{
    SC_METHOD(process);
    sensitive << clock.pos();
    g_ = 0.0*si::siemens;
}


void ExpSynapse::process() 
{
    currTime_ = sc_time_stamp().to_seconds() * si::second;

    vPre_ = pre.read() * si::volt;
    vPost_ = post.read()*si::volt;

    if(vPre_ >= Esyn_)
        ts_ = currTime_;

    if(ts_ == 0.0*si::second)
        return;

    // Now compute gsyn.
    auto dt = (currTime_ - ts_);
    assert(tau1_ > 0.0*si::second);

    g_ = gbar_ * tantrika::alpha(quantity_cast<double>(dt)
            , quantity_cast<double>(tau1_)
            );

    // cout << '\t' << g_ << " " << dt << " " << tau1_ << " " << endl;
    inject.write(quantity_cast<double>(g_*(vPost_-Esyn_)));
}


// =====================================================================================
//
//       Filename:  IAF.cpp
//
//    Description:  Integrate and Fire neuron.
//
//        Version:  1.0
//        Created:  Saturday 27 April 2019 04:51:37  IST
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Dilawar Singh (), dilawar.s.rajput@gmail.com
//   Organization:  NCBS Bangalore
//
// =====================================================================================

#include "IAF.h"
#include <systemc.h>

IAF::IAF(sc_module_name name, double em, double tau)
    : name_(name) 
      , Cm_(1e-6)
      , Em_(em)
      , vm_(em)
      , tau_(tau)
{
    Rm_ = tau_/Cm_;
    init();
}

IAF::IAF(sc_module_name name, double em, double cm, double rm)
    : name_(name) 
      , Cm_(cm)
      , Em_(em)
      , Rm_(rm)
      , vm_(em)
{
    tau_ = Rm_*Cm_;
    init();
}

void IAF::init()
{
    spikes_.clear();
    data_.clear();

    SC_METHOD(decay);
    sensitive << clock.pos();

    SC_METHOD(handleInjection);
    sensitive << nonZeroInject;

    t_ = sc_time_stamp().to_seconds();
    prevT_ = t_;
    threshold_ = Em_ + 10e-3;
    refactory_ = 0.0;
    fired_ = false;
}

std::string IAF::repr()
{
    std::stringstream ss;
    ss << boost::format("IAF:%1%, Em=%2%, cm=%3% rm=%4% tau=%5% refactory=%6% threshold=%7%") 
        % name_ % Em_ % Cm_ % Rm_ % tau_ % refactory_ % threshold_;
    return ss.str();
}

void IAF::setRefactory(const double ref)
{
    assert(ref >= 0.0);
    refactory_ = ref;
}

void IAF::setThreshold(const double thres)
{
    assert(thres > Em_);
    threshold_ = thres;
}


void IAF::setTau(const double tau)
{
    assert(tau > 0.0);
    tau_ = tau;
}


void IAF::model(const double &vm, double &vmdt, const double t)
{
    vmdt = (-vm+Em_ + inject.read()*Rm_)/tau_;
}

void IAF::decay()
{
    t_ = sc_time_stamp().to_seconds();

    dt_ = t_ - prevT_;
    if(dt_ == 0.0)
        return;


    if(fired_)
    {
        vm_ = Em_;

        // Reset fired to false only after refactory period.
        if(t_ - spikes_.back() >= refactory_)
            fired_ = false;
    }

    // Inject only when fired_ is set to false or inject value is non-zero.
    if(inject != 0.0 && (! fired_))
        nonZeroInject.notify();

    vm_ += dt_*(-vm+Em_)/tau_;
    if(vm_ >= threshold_)
    {
        vm_ = 10e-3;
        spikes_.push_back(t_);
        fired_ = true;
    }
    vm.write(vm_);
    prevT_ = t_;
}

void IAF::handleInjection()
{
    double dvdt = (inject * dt_)/Cm_;
    vm_ += dvdt;
    // cout << " Injected " << inject << " dv " << dvdt << " vm = " << vm_;
    // cout << '\t' << repr() << endl;
}

void IAF::addSynapse(Synapse& syn)
{
    // BOOST_LOG_TRIVIAL(debug) << "Added synapse " << syn << " to " << name_;
    synapses_.push_back( std::shared_ptr<Synapse>(&syn) );
}

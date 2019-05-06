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

#include <boost/format.hpp>

#include "IAF.h"
#include "../utility/data_util.h"


IAF::IAF(sc_module_name name, double em, double tau) :
      Cm_(100e-12)                            // 100 pF
      , Em_(em)
      , vm_(em)
      , tau_(tau)
{
    name_ = string(name);
    Rm_ = tau_/Cm_;
    init();
}

IAF::IAF(sc_module_name name, double em, double cm, double rm)
    : Cm_(cm)
      , Em_(em)
      , Rm_(rm)
      , vm_(em)
{
    name_ = string(name);
    tau_ = Rm_*Cm_;
    init();
}

void IAF::init()
{
    spikes_.clear();
    data_.clear();

    noise_ = 0.0;
    std::random_device rd;
    gen_.seed( rd() );

    SC_METHOD(record);
    sensitive << clock.neg();

    SC_METHOD(decay);
    sensitive << clock.pos();

    SC_METHOD(handleInjection);
    sensitive << nonZeroInject;

    SC_METHOD(handleSynapticInjection);
    sensitive << nonZeroSynCurrent;

    SC_METHOD(handleOnFire);
    sensitive << onFire;

    t_ = sc_time_stamp().to_seconds();
    prevT_ = t_;
    threshold_ = Em_ + 10e-3;
    refactory_ = 0.0;
    fired_ = false;
}

void IAF::record( void )
{
    t_ = sc_time_stamp().to_seconds();
    data_.push_back( make_tuple(t_, vm.read()));
}

void IAF::setNoise( double eps)
{
    assert(eps >= 0.0);
    noise_ = eps;
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

void IAF::handleOnFire()
{
    // reset.
    vm_ = Em_;
}

double IAF::noise()
{
    if(noise_ == 0.0)
        return 0.0;
    return noise_*dist_(gen_);
}

void IAF::decay()
{
    t_ = sc_time_stamp().to_seconds();

    dt_ = t_ - prevT_;
    if(dt_ == 0.0)
        return;

    // Refactory period.
    if(! spikes_.empty() > 0 && t_ - spikes_.back() <= refactory_)
        return;

    // Inject only when fired_ is set to false or inject value is non-zero.
    if((inject != 0.0) && (! fired_))
        nonZeroInject.notify();

    // Collect currents from synspases.
    sum_all_synapse_inject_ = 0.0;
    for (size_t i = 0; i < synapses_.size(); i++) 
        sum_all_synapse_inject_ += synapse_inject[i].read();

    if(sum_all_synapse_inject_ != 0.0)
        nonZeroSynCurrent.notify();

    vm_ += dt_*(-vm+Em_+noise())/tau_;
    if(vm_ >= threshold_)
    {
        vm_ = 40e-3;
        spikes_.push_back(t_);
        onFire.notify();
    }
    vm.write(vm_);
    prevT_ = t_;
}

void IAF::handleSynapticInjection()
{
    // cout << " total injection " << sum_all_synapse_inject_ << endl;
    vm_ += (-sum_all_synapse_inject_ * dt_)/Cm_;
}

void IAF::handleInjection()
{
    vm_ += (inject * dt_)/Cm_;
}

void IAF::addSynapse(shared_ptr<SynapseBase> syn)
{
    // BOOST_LOG_TRIVIAL(debug) << "Added synapse " << syn << " to " << name_;
    syn->post(vm);
    syn->clock(clock);
    syn->inject(synapse_inject[synapses_.size()]);
    synapses_.push_back(syn);
}

std::vector<std::tuple<double, double>> IAF::data() const
{
    return data_;
}

void IAF::save_data(const string& outfile)
{
    // If outfile is empty, then write to nauron name.
    string filename(outfile);
    if(outfile.size() == 0)
        filename = name_ + ".csv";

    write_to_csv(data_, filename, "time, vm");
}

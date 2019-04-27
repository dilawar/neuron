/*
 *    Description:  
 *
 *         Author:  Dilawar Singh (), dilawars@ncbs.res.in
 *   Organization:  NCBS Bangalore
 */

#include <memory>
#include <sstream>
#include <functional>
#include <boost/format.hpp>
#include <boost/log/trivial.hpp>
#include <boost/numeric/odeint.hpp>

#include "Synapse.h"
#include "../engine/engine.h"

using namespace std;
using namespace boost;
namespace odeint = boost::numeric::odeint;

typedef odeint::runge_kutta_cash_karp54< state_type > error_stepper_type;

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
    sensitive << clock.pos();

    SC_METHOD(monitor_spike);
    sensitive << pre;

    g_ = 0.0*si::siemens;
    BOOST_LOG_TRIVIAL(debug) << repr();
}

/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  A synapse which is solved by ODE solver. May be more efficient in
 * some cases.
 *
 * NOTE: Detect the spike in the same function. DO NOT use monitor_spike
 * function since it may execute in any order and we may not be able to store
 * the right value of spike in the vector.
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
    state_[0] = 0.0; //g_/(1*si::siemens);
    state_[1] = 0.0; //(1*si::siemens);

    t_ = 0.0*si::second;
    prevT_ = 0.0*si::second;

    odeSys_ = std::make_unique<SynapseODESystem>(gbar_, tau1_, tau2_);
    BOOST_LOG_TRIVIAL(debug) << repr();

    SC_THREAD( tickOdeClock );

    // Make it sensitive to pre as well. Otherwise we will not collect spikes.
    SC_METHOD(processODE);
    sensitive << pre << ode_clock;

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
 * conductance. DO NOT USE IT IN ODE solver.
 */
/* ----------------------------------------------------------------------------*/
void Synapse::monitor_spike( )
{
    auto t = sc_time_stamp().to_seconds() * si::second;
    vPost_ = post.read()*si::volt;

    // Time of previous spike.
    if(pre.read() == true)
        t_spikes_.push_back(t);
}

void Synapse::injectCurrent( )
{
    inject.write(quantity_cast<double>(g_*(vPost_-Esyn_)));
}

void Synapse::processAlpha() 
{
    t_ = sc_time_stamp().to_seconds() * si::second;
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
    // dt_ = (channel ? channel->period() : SC_ZERO_TIME).to_seconds()*si::second;
} 

void Synapse::printODEData()
{
    for(auto v : data_)
        cout << std::get<0>(v) << ' ' << std::get<1>(v) << endl;
}

/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  Solve the simple using ODE solver. 
 * To make computation faster, tick the solver when there is spike (positive edge).
 *
 * This function does not always tick at fixed dt.
 */
/* ----------------------------------------------------------------------------*/
void Synapse::processODE() 
{
    t_ = sc_time_stamp().to_seconds() * si::second;
    double dt = quantity_cast<double>(t_-prevT_);
    std::cout << "Warn: This method does not work very well." << std::endl;

    // cout << 'x' << t_ << ' ' << prevT_ << ' ' << dt << ' ' << gbar_ << endl;

    if( dt == 0.0)
        return;

    // Make sure we put the spike into ODE system.
    if(true == pre.read())
    {
        t_spikes_.push_back(t_);
        odeSys_->addSpike(t_);
    }


    //// std::cout << "Calling ODE process " << sc_time_stamp() << std::endl;
    //cout << boost::format("%1%: time: %2%  prevtime: %3% state: %4% %5%, spikes %6% "
    //        ) % name_ % t_ % prevT_ % state_[0] % state_[1] % t_spikes_.size(); 
    //for(auto spk: t_spikes_) cout << spk << ' ';
    //cout << endl;

#if 1
    size_t n = odeint::integrate_adaptive( 
            // rk4_stepper_type_()
            odeint::make_controlled<rk_dopri_stepper_type_>( 1e-6, 1e-3 )
            // rk_dopri_stepper_type_()
            // rk_karp_stepper_type_()
            // rk_felhberg_stepper_type_()
            , [this](const state_type &dy, state_type &dydt, double t) {
                this->odeSys_->alphaSynapse(dy, dydt, t); 
            }
            , state_
            , quantity_cast<double>(prevT_/si::second)
            , quantity_cast<double>(t_/si::second)
            , 1e-5
            // , synapse_observer(data_)
            );
#else
    size_t n = odeint::integrate( 
            [this](const state_type &dy, state_type &dydt, double t) {
                this->odeSys_->alphaSynapse(dy, dydt, t); 
            }
            , state_
            , quantity_cast<double>(prevT_/si::second)
            , quantity_cast<double>(t_/si::second)
            , dt
            // , synapse_observer(data_)
            );

#endif
    cout << " #" << n <<  ' ' << state_[1]/state_[0] << ' ' << dt << endl;
    cout.flush();

    prevT_ = t_;
    g_ = state_[0] * si::siemens;
    injectCurrent();
}


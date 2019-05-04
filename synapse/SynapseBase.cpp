/*
 *    Description:  
 *
 *         Author:  Dilawar Singh (), dilawars@ncbs.res.in
 *   Organization:  NCBS Bangalore
 */

#include <memory>
#include <sstream>
#include <functional>
#include <boost/range/adaptor/reversed.hpp>
#include <boost/format.hpp>

#include <boost/log/trivial.hpp>
#include <boost/numeric/odeint.hpp>

#include "../include/SynapseBase.h"
#include "../engine/engine.h"
#include "../utility/data_util.h"

using namespace std;
using namespace boost;
namespace odeint = boost::numeric::odeint;

typedef odeint::runge_kutta_cash_karp54< state_type > error_stepper_type;

SynapseBase::SynapseBase(sc_module_name name, double gbar, double tau, double Esyn):
    name_(name) 
    , gbar_(gbar)
    , tau1_(tau)
    , Esyn_(Esyn)
{
#if 0
    SC_METHOD(process)
    sensitive << clock.pos();
#endif

    SC_METHOD(monitor_spike);
    sensitive << spike;

    g_ = 0.0;
    BOOST_LOG_TRIVIAL(debug) << repr();
}

SynapseBase::~SynapseBase()
{}


std::string SynapseBase::repr()
{
    std::stringstream ss;
    ss << boost::format("SYNAPSE:%1%, gbar=%2%, Esyn=%3% tau1=%4% tau2=%5%\n") 
        % name_ % gbar_ % Esyn_ % tau1_ % tau2_;
    return ss.str();
}

std::string SynapseBase::name()
{
    return name_;
}

#if 0

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
SynapseBase::SynapseBase(sc_module_name name, double gbar, double tau1, double tau2, double Esyn): 
    name_(name) 
    , gbar_(gbar)
    , tau1_(tau1)
    , tau2_(tau2)
    , Esyn_(Esyn)
{
    g_ = 0.0;
    state_[0] = 0.0; 
    state_[1] = 0.0; 

    t_ = 0.0;
    prevT_ = 0.0;

    // Make it sensitive to spike as well. Otherwise we will not collect spikes.
    SC_METHOD(processODE);
    sensitive << spike;

}
#endif

/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  Helper function to do few operations before we compute the
 * conductance. DO NOT USE IT IN ODE solver.
 */
/* ----------------------------------------------------------------------------*/
void SynapseBase::monitor_spike( )
{
    auto t = sc_time_stamp().to_seconds();
    vPost_ = post.read();

    // Time of previous spike.
    if(spike.read() == true)
        t_spikes_.push_back(t);
}

#if 0
void SynapseBase::injectCurrent( )
{
    inject.write(g_*(vPost_-Esyn_));
}

void SynapseBase::processSingleExp() 
{
    t_ = sc_time_stamp().to_seconds();
    g_ = 0.0;

    // Any spike which occured 10*tau_ before is not worth computing.
    for (auto tSpike : boost::adaptors::reverse(t_spikes_))
    {
        if(tSpike < t_)
        {
            auto T = (t_-tSpike)/tau1_;
            g_ += gbar_ * exp(-T);
            if( T > 10)
                break;
        }
    }

    data_.push_back(std::make_pair(t_, g_));

    injectCurrent();
}

void SynapseBase::processAlpha() 
{
    t_ = sc_time_stamp().to_seconds();
    g_ = 0.0;

    // Any spike which occured 10*tau_ before is not worth computing.
    for (auto tSpike : boost::adaptors::reverse(t_spikes_))
    {
        if(tSpike < t_)
        {
            auto T = (t_-tSpike)/tau1_;
            g_ += gbar_ * T * exp(-T);
            if( T > 10)
                break;
        }
    }

    data_.push_back(std::make_pair(t_, g_));

    injectCurrent();
}

/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  Start of simulation.
 */
/* ----------------------------------------------------------------------------*/

void SynapseBase::printODEData()
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
void SynapseBase::processODE() 
{
    t_ = sc_time_stamp().to_seconds();
    double dt = (t_-prevT_);
    std::cout << "Warn: This method does not work very well." << std::endl;

    if( dt == 0.0)
        return;

    // Make sure we put the spike into ODE system.
    if(true == spike.read())
    {
        t_spikes_.push_back(t_);
        odeSys_->addSpike(t_);
    }

#if 1
    size_t n = odeint::integrate_adaptive( 
            // rk4_stepper_type_()
            odeint::make_controlled<rk_dopri_stepper_type_>( 1e-6, 1e-3 )
            // rk_dopri_stepper_type_()
            // rk_karp_stepper_type_()
            // rk_felhberg_stepper_type_()
            , [this](const state_type &dy, state_type &dydt, double t) {
                this->odeSys_->alphaSynapseBase(dy, dydt, t); 
            }
            , state_, prevT_, t_, 1e-5
            // , synapse_observer(data_)
            );
#else
    size_t n = odeint::integrate( 
            [this](const state_type &dy, state_type &dydt, double t) {
                this->odeSys_->alphaSynapseBase(dy, dydt, t); 
            }
            , state_
            , quantity_cast<double>(prevT_)
            , quantity_cast<double>(t_)
            , dt
            // , synapse_observer(data_)
            );

#endif
    cout << " #" << n <<  ' ' << state_[1]/state_[0] << ' ' << dt << endl;
    cout.flush();

    prevT_ = t_;
    g_ = state_[0];
    injectCurrent();
}
#endif

void SynapseBase::save_data(const std::string& filename)
{
    string outfile(filename);
    if(filename.size() < 1)
        outfile = name_ + ".csv";
    write_to_csv(data_, outfile, "time, g");
}

const std::vector<std::tuple<double, double> >* SynapseBase::data() const
{
    return &data_;
}

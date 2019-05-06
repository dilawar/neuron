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

SynapseBase::SynapseBase(sc_module_name name, double gbar, double tau, double Esyn):
    name_(name) 
    , gbar_(gbar)
    , tau1_(tau)
    , Esyn_(Esyn)
{
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
    return string(name_);
}

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

void SynapseBase::injectCurrent( )
{
    inject.write(g_*(vPost_-Esyn_));
}

#if 0
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
        outfile = name() + ".csv";
    write_to_csv(data_, outfile, "time, g");
}

const std::vector<std::tuple<double, double> >* SynapseBase::data() const
{
    return &data_;
}

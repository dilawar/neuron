// =====================================================================================
//
//       Filename:  OdeSystem.h
//
//    Description:  Ode System for synapse.
//
//        Version:  1.0
//        Created:  Tuesday 23 April 2019 03:16:50  IST
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Dilawar Singh (), dilawar.s.rajput@gmail.com
//   Organization:  NCBS Bangalore
//
// =====================================================================================

#ifndef ODESYSTEM_H
#define ODESYSTEM_H

#include <algorithm>
#include <iostream>
#include <array>
#include <vector>
#include <tuple>

#include <boost/numeric/odeint.hpp>
#include <boost/log/trivial.hpp>
#include <boost/format.hpp>

typedef std::array<double, 2> state_type;

// state_type is in OdeSystem.h
typedef boost::numeric::odeint::runge_kutta4< state_type > rk4_stepper_type_;
typedef boost::numeric::odeint::runge_kutta_dopri5< state_type > rk_dopri_stepper_type_;

/*-----------------------------------------------------------------------------
 *  This stepper type found to be most suitable for adaptive solver. The gsl
 *  implementation has runge_kutta_fehlberg78 solver.
 *-----------------------------------------------------------------------------*/
typedef boost::numeric::odeint::runge_kutta_cash_karp54< state_type > rk_karp_stepper_type_;
typedef boost::numeric::odeint::runge_kutta_fehlberg78< state_type > rk_felhberg_stepper_type_;


struct synapse_observer
{
    std::vector< std::tuple<double, double> >& m_data;

    synapse_observer( std::vector<std::tuple<double, double> >& states): m_data( states ) 
    { }

    void operator()( const state_type &x , double t )
    {
        m_data.push_back( std::make_pair(t, x[0]));
    }
};

struct SynapseODESystem
{
    SynapseODESystem(double gbar,  double tau1, double tau2) 
        : gbar(gbar), tau1(tau1), tau2(tau2)
    {
        spikes.clear();
    }

    double spike(const double t)
    {
        double val = 1.0;
    #if 1
        // Return 1 if there is a spike in spikeVector.
        if(spikes.end() == std::find(spikes.begin(), spikes.end(), t))
            val = 0.0;
    #else
        if( spikes.size() > 0 && t >= spikes[0])
        {
            spikes.erase(spikes.begin());
            val = 1.0;
        }
        val = 0.0;
#endif
        return val;
    }

    void addSpike(const double t)
    {
        // cout << "Adding spike";
        spikes.push_back(t);
    }

    void addSpikes(const std::vector<double> spks)
    {
        spikes.insert(spikes.end(), spks.begin(), spks.end());
    }

    // When tau1 and tau2 are equal, this is equivalent to alpha synapse.
    // Otherwise the solution would not match the alpha synapse.
    void alphaSynapse(const state_type &x, state_type& dxdt, const double t)
    {
        double _gscale = tau1;
        int spikeVal = spike(t);

        dxdt[0] = x[1];
        dxdt[1] = (-x[0] - (tau1+tau2)*x[1] + gbar/_gscale*spikeVal)/tau1/tau2;

        if(spikeVal > 0)
            BOOST_LOG_TRIVIAL(info) << "Spike @" << t << ' ' << spikeVal << endl;
    }

    double gbar;
    double tau1, tau2;
    std::vector<double> spikes;

    double epsRel = 1e-6;
    double epsAbs = 1e-6;

};


#endif /* end of include guard: ODESYSTEM_H */

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

#include <boost/units/systems/si.hpp>
#include <boost/units/systems/si/io.hpp>
#include <boost/numeric/odeint.hpp>
#include <boost/log/trivial.hpp>
#include <boost/format.hpp>

using namespace boost::units;

typedef std::array<double, 2> state_type;

// state_type is in OdeSystem.h
typedef boost::numeric::odeint::runge_kutta4< state_type > rk4_stepper_type_;
typedef boost::numeric::odeint::runge_kutta_dopri5< state_type > rk_dopri_stepper_type_;
typedef boost::numeric::odeint::modified_midpoint< state_type > rk_midpoint_stepper_type_;

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
    SynapseODESystem( quantity<si::conductance> gbar, quantity<si::time> tau1, quantity<si::time> tau2) 
        : gbar(gbar), tau1(tau1), tau2(tau2)
    {
        spikes.clear();
    }

    double spike(const quantity<si::time> t)
    {
        // Return 1 if there is a spike in spikeVector.
        if(spikes.end() == std::find(spikes.begin(), spikes.end(), t))
            return 0.0;
        return 1.0;
    }

    void addSpike(const quantity<si::time> t)
    {
        // cout << "Adding spike";
        spikes.push_back(t);
    }

    void addSpikes(const std::vector<quantity<si::time>> spks)
    {
        spikes.insert(spikes.end(), spks.begin(), spks.end());
    }

    // This is for ODE system.
    void systemSynapticConductance(const state_type &x, state_type& dxdt, const double t)
    {
        double gb = gbar/si::siemens;
        double _tau1 = tau1/si::second;
        double _tau2 = tau2/si::second;

        dxdt[0] = x[1];
        dxdt[1] = (-x[0] - (_tau1+_tau2)*x[1] + gb*spike(t*si::second))/_tau1/_tau2;

        // std::cout << "  @" << t << ' ' << spike(t*si::second);
        //std::cout << boost::format( "@t%5% %1% dydy %2% tau1 %3% tau2 %4%\n"
        //        ) % dxdt[0] % dxdt[1] % tau1 % tau2 % t;
    }

    quantity<si::conductance> gbar;
    quantity<si::time> tau1, tau2;
    std::vector<quantity<si::time>> spikes;

    double epsRel = 1e-6;
    double epsAbs = 1e-6;

};


#endif /* end of include guard: ODESYSTEM_H */

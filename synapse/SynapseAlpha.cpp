// =====================================================================================
//
//       Filename:  SynapseAlpha.cpp
//
//    Description:  Single Exponential Synapse.
//
//        Version:  1.0
//        Created:  Friday 03 May 2019 04:43:02  IST
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Dilawar Singh (), dilawar.s.rajput@gmail.com
//   Organization:  NCBS Bangalore
//
// =====================================================================================

#include "SynapseAlpha.h"

#include <boost/range/adaptor/reversed.hpp>

SynapseAlpha::SynapseAlpha(sc_module_name name, double gbar, double tau, double Esyn) :
    SynapseBase(name, gbar, tau, Esyn)
{
    SC_METHOD(alpha_synapse_process);
    sensitive << clock.pos();
}

void SynapseAlpha::alpha_synapse_process()
{
    process();
}

void SynapseAlpha::process()
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
    SynapseBase::injectCurrent();
}

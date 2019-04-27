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

#include <systemc>
#include "IAF.h"


IAF::IAF(sc_module_name name, double vm, double tau)
    : name_(name) 
      , vm_(vm*si::volt)
      , tau_(tau*si::second)
{
    spikes_.clear();
    data_.clear();
}

void IAF::process()
{
    std::cout << "Process " << sc_time_stamp() << std::endl;
}

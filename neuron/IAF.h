// =====================================================================================
//
//       Filename:  IAF.h
//
//    Description:  Integrate and fire neuron.
//
//        Version:  1.0
//        Created:  Saturday 27 April 2019 04:52:03  IST
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Dilawar Singh (), dilawar.s.rajput@gmail.com
//   Organization:  NCBS Bangalore
//
// =====================================================================================

#ifndef IAF_H
#define IAF_H

#include <systemc.h>
#include "../synapse/Synapse.h"
#include <boost/units/systems/si.hpp>
#include <boost/units/systems/si/io.hpp>
#include <vector>
#include <tuple>
#include <array>
#include <memory>

using namespace boost::units;
namespace si = boost::units::si;


using namespace std;

class IAF : public sc_module
{
    public:
        SC_HAS_PROCESS(IAF);
        sc_in_clk clock;

        // Ports.

        void process( );

        IAF(sc_module_name name, double vm=-65e-3, double tau=1e-3);

    public:
        sc_module_name name_;
        quantity<si::electric_potential> vm_;
        quantity<si::time> tau_;
        std::vector<std::tuple<si::time, si::electric_potential> > data_;
        std::vector<si::time> spikes_;
};

#endif /* end of include guard: IAF_H */

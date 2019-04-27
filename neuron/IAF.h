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

using namespace std;

class IAF : public sc_module
{
    public:
        SC_HAS_PROCESS(IAF);
        sc_in_clk clock;

        // Ports.

        void process( );

        IAF(sc_module_name name);

    public:
        sc_module_name name_;
};

#endif /* end of include guard: IAF_H */

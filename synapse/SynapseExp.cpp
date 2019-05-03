// =====================================================================================
//
//       Filename:  SynapseExp.cpp
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

#include "../SynapseExp.h"

/* Alpha synapse */
SynapseExp::SynaspeExp(sc_module_name name, double gbar, double tau, double Esynl)
{

}

/* Dual exp synapse (use ode solver) */
SynapseExp::SynaspeExp(sc_module_name name, double gbar, double tau1, double tau2, double Esyn)
{

}

void SynaspeExp::process()
{
    cout <<  " process " << endl;
}

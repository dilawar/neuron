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

#include "SynapseExp.h"

SynapseExp::SynapseExp(sc_module_name name, double gbar, double tau, double Esyn) :
    SynapseBase(name, gbar, tau, Esyn)
{}

void SynapseExp::process()
{
    cout <<  " process " << endl;
}

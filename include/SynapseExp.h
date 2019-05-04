/***
 *    Description:  Single Exponential class.
 *
 *         Author:  Dilawar Singh <dilawars@ncbs.res.in>
 *   Organization:  NCBS Bangalore
 *        License:  MIT License
 */

#ifndef SYNAPSEEXP_H
#define SYNAPSEEXP_H

#include "SynapseBase.h"

#include <systemc>
#include <vector>
#include <tuple>
#include <array>
#include <memory>

/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  SynaspeExp class.
 *
 */
/* ----------------------------------------------------------------------------*/
class SynapseExp: public SynapseBase
{

public:
    SC_HAS_PROCESS(SynapseExp);


    //-----------------------------------------------------------------------------
    //  Constructors.
    //-----------------------------------------------------------------------------
    SynapseExp(sc_module_name name, double gbar, double tau, double Esyn);

    /*-----------------------------------------------------------------------------
     *  At each tick, these process function computes the model.
     *-----------------------------------------------------------------------------*/
    void process() override;


#if 0
    /* Alpha synapse */
    SynapseExp(sc_module_name name, double gbar, double tau, double Esynl, bool isalpha=true);

    /* Dual exp synapse (use ode solver) */
    SynapseExp(sc_module_name name, double gbar, double tau1, double tau2, double Esyn
               , double odedt=3e-5
              );
#endif

};

#endif /* end of include guard: SYNAPSEEXP_H */

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
    SynapseExp(const std::string& name, double gbar=1e-9, double tau=1e-3, double Esyn=0.0);

    /*-----------------------------------------------------------------------------
     *  At each tick, these process function computes the model.
     *-----------------------------------------------------------------------------*/
    void process() override;

};

#endif /* end of include guard: SYNAPSEEXP_H */

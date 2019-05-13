/***
 *    Description:  Single Exponential class.
 *
 *         Author:  Dilawar Singh <dilawars@ncbs.res.in>
 *   Organization:  NCBS Bangalore
 *        License:  MIT License
 */

#ifndef SYNAPSEALPHA_H
#define SYNAPSEALPHA_H




#include "SynapseBase.h"

#include <systemc>
#include <vector>
#include <tuple>
#include <array>
#include <memory>

/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  SynapseAlpha class.
 *
 */
/* ----------------------------------------------------------------------------*/
class SynapseAlpha: public SynapseBase
{

public:
    SC_HAS_PROCESS(SynapseAlpha);

    //-----------------------------------------------------------------------------
    //  Constructors.
    //-----------------------------------------------------------------------------
    SynapseAlpha(sc_module_name name, double gbar, double tau, double Esyn);

    /*-----------------------------------------------------------------------------
     *  At each tick, these process function computes the model.
     *-----------------------------------------------------------------------------*/
    void alpha_synapse_process();
    void process() override;

};

#endif /* end of include guard: SYNAPSEALPHA_H */

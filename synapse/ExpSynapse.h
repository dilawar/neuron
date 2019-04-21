/*
 * =====================================================================================
 *
 *       Filename:  ExpSynapse.h
 *
 *    Description:  Exponential synapse.
 *
 *        Version:  1.0
 *        Created:  Sunday 21 April 2019 04:30:49  IST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Dilawar Singh (), dilawars@ncbs.res.in
 *   Organization:  NCBS Bangalore
 *
 * =====================================================================================
 */

#ifndef EXPSYNAPSE_H
#define EXPSYNAPSE_H

#include "systemc.h"

SC_MODULE(ExpSynapse) 
{
    sc_in_clk clock;
    sc_in<bool> pre_synaptic_event;
    sc_out<bool> post_synaptic_event;

    void process() 
    {
        cout << "process " << pre_synaptic_event << endl;
    }

    SC_CTOR(ExpSynapse) 
    {
        SC_METHOD(process);
        sensitive << clock.pos();
        sensitive << pre_synaptic_event;
    }
};

#endif /* end of include guard: EXPSYNAPSE_H */

/*
 * =====================================================================================
 *
 *       Filename:  test_exp_syn.cpp
 *
 *    Description:  test synapse.
 *
 *        Version:  1.0
 *        Created:  Sunday 21 April 2019 04:35:29  IST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Dilawar Singh (), dilawars@ncbs.res.in
 *   Organization:  NCBS Bangalore
 *
 * =====================================================================================
 */

#include "systemc.h"

#include "ExpSynapse.h"
#include <random>
#include <chrono>

int testExpSyn()
{
    // Calcium pulse into syanpse.
    sc_signal<bool> in, out;
    sc_clock clock("clock", 1, SC_MS);

    ExpSynapse syn1("ExpSynapse");

    syn1.clock(clock);
    syn1.pre_synaptic_event(in);
    syn1.post_synaptic_event(out);

    sc_start(10, SC_MS);

    return(0);
}

int sc_main(int argc, char *argv[])
{
    return testExpSyn();
}

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

SC_MODULE(TestExpSyn) 
{
    sc_in<bool> clock;

    // Calcium pulse into syanpse.
    sc_signal<bool> in, out;

    void do_test() 
    {
        cout << "Test " << endl;
    }

    SC_CTOR(TestExpSyn) {
        SC_METHOD(do_test);
        sensitive << clock.neg();

        // dut
        // ExpSynapse dut("ExpSynapse");
        // dut.pre_synaptic_event(in);
        // dut.post_synaptic_event(out);
    }
};

int sc_main(int argc, char *argv[])
{
    sc_clock clock("clock", 1, SC_MS);

    TestExpSyn tb("TestBench");
    tb.clock(clock);

    sc_start(20, SC_MS);

    return 0;
}

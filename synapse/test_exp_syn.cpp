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
#include <random>

SC_MODULE(TestExpSyn) 
{
    sc_in<bool> clock;

    // Spike goes into synapse.
    sc_signal<bool> in;

    // A voltage comes out of synapse.
    sc_signal<double> out;

    void do_test() 
    {
        while(true)
        {
            wait(dist_(gen_), SC_MS);
            in = true;
            wait(1, SC_MS);
            in = false;
        }
    }

    void process()
    {
        std::cout << in <<  ' ' << std::endl;

    }

    SC_CTOR(TestExpSyn) 
    {
        SC_THREAD(do_test);

        SC_METHOD(process);
        sensitive << clock.neg();

        // dut
        // ExpSynapse dut("ExpSynapse");
        // dut.pre_synaptic_event(in);
        // dut.post_synaptic_event(out);
        gen_.seed(rd_());
        dist_.param(std::poisson_distribution<int>::param_type {5});
    }

    // Methods


    // Data members.
    std::random_device rd_;
    std::mt19937 gen_;
    std::poisson_distribution<> dist_;

};

int sc_main(int argc, char *argv[])
{
    sc_clock clock("clock", 1, SC_MS);

    TestExpSyn tb("TestBench");
    tb.clock(clock);

    sc_start(20, SC_MS);

    return 0;
}

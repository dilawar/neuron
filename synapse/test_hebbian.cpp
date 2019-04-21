/***
 *    Description:  Test all the synapses.
 *
 *        Created:  2019-04-21

 *         Author:  Dilawar Singh <dilawars@ncbs.res.in>
 *   Organization:  NCBS Bangalore
 *        License:  MIT License
 */

#include "Hebbian.h"
#include <random>
#include <chrono>

int test_simple_synapse()
{
    /*  Create a global signal */
    sc_signal<bool> reset;
    sc_signal<bool> enable;
    sc_signal<bool> event;

    // Calcium pulse into syanpse.
    sc_signal<double> ca_in;

    sc_signal<bool> pre_input, post_input;

    sc_clock TestClk("TestClock", 1, SC_MS);

    SimpleSynapse syn1("Synapse");
    syn1.clock(TestClk);
    syn1.reset(reset);
    syn1.enable(enable);

    syn1.pre_synaptic_event(pre_input);
    syn1.post_synaptic_event(post_input);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::discrete_distribution<> dpre({10,50});
    std::discrete_distribution<> dpost({300,10});
    
    /* Run forever */
    for (int i = 0; i < 10000; i++)
    {
        pre_input = dpre(gen);
        post_input = dpost(gen);
        sc_start(1, SC_MS);
    }

    return(0);
}

int sc_main(int argc, char *argv[])
{
    return test_simple_synapse();
}

/*
Copyright (C) 2015 Dilawar Singh

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, see <http://www.gnu.org/licenses/>.

*/

#include "simple_synapse.h"
#include "input_types.h"

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

    LOG("INFO", "Ran for " << sc_time_stamp());
    return(0);
}


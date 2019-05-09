/***
 *    Description:  Test network.
 *
 *        Created:  2019-05-08

 *         Author:  Dilawar Singh <dilawars@ncbs.res.in>
 *   Organization:  NCBS Bangalore
 *        License:  MIT License
 */

#include "../include/tantrika.h"
#include <systemc>

int main(int argc, const char *argv[])
{
    Network net("A", 1e-3);

    // Add input.
    net.addSpikeGeneratorPeriodicGroup("A/i1", 10, 1e-3, 0.0);

    // Add synapses.
    net.addSynapseGroup("A/s1", 10, 1e-9, 1e-3, 0.0);
    net.addNeuronGroup("A/n1", 10);

    net.connect("A/i1", "output", "A/s1", "spike");
    net.connect("A/n1", "vm", "A/s1", "post");  // neuron vm are connected to synapse post.

    net.start(10);
    
    return 0;
}


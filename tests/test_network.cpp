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
    net.addSpikeGeneratorPeriodicGroup("i1", 10, 1e-3, 0.0);

    // Add synapses.
    net.addSynapseGroup("s1", 10, 1e-9, 1e-3, 0.0);
    net.addNeuronGroup("n1", 10);

    net.connect("i1", "output", "s1", "spike");
    net.connect("n1", "vm", "s1", "post");  // neuron vm are connected to synapse post.

    net.start(10);
    
    return 0;
}


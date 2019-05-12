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
    Network net("A", 1e-4);

    // Add input.
    net.addSpikeGeneratorPeriodicGroup("i1", 2, 1e-3, 0.0);

    // Add synapses.
    net.addSynapseGroup("s1", 2, 1e-9, 1e-3, 0.0);
    net.addNeuronGroup("n1", 2);

    // Spike of SpikeGenerator goes into spike of SynapseGroup 
    net.connect("i1", "output", "s1", "spike");

    // psc of SpikeGenerator goes to inject of neuron.
    net.connect("s1", "psc", "n1", "inject");
    net.connect("n1", "vm", "s1", "post");  // neuron vm are connected to synapse post.

    net.start(5e-3);
    
    return 0;
}


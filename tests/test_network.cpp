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
    Network net("A", 1e-5);

    // Add input.
    net.addSpikeGeneratorPeriodicGroup("i1", 2, 1e-3, 0.0);

    // Add synapses.
    net.addSynapseGroup("s1", 2, 1e-9, 1e-3, 0.0);
    net.addNeuronGroup("n1", 2);

    // Spike of SpikeGenerator goes into spike of SynapseGroup 
    net.connect("i1", "output", "s1", "spike");

    // neuron vm are connected to synapse post.
    net.connect("n1", "vm", "s1", "post"); 

    // psc of SpikeGenerator goes to inject of neuron.
    net.connect("s1", "psc", "n1", "psc");

    net.start(10e-3);
    net.saveData();
    
    return 0;
}


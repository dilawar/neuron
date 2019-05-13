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
    size_t N = 2;

    // Add input.
    net.addSpikeGeneratorPeriodicGroup("i1", N, 5e-3, 3e-3);
    net.addSpikeGeneratorPeriodicGroup("i2", N, 5e-3, 3e-3);

    // neuron groups.
    net.addNeuronGroup("n1", N, 1e-12, 100e6, -65e-3);

    // Add synapses.
    net.addSynapseGroup("exc", N, 5e-9, 1e-3, 0.0);
    net.addSynapseGroup("inh", N, 1e-9, 5e-3, -70e-3);

    // Spike of SpikeGenerator goes into spike of SynapseGroup 
    net.connect("i1", "output", "exc", "spike");
    net.connect("i2", "output", "inh", "spike");

    // neuron vm are connected to synapse post.
    net.connect("n1", "vm", "exc", "post"); 
    net.connect("n1", "vm", "inh", "post"); 

    // psc of SpikeGenerator goes to inject of neuron.
    net.connect("exc", "psc", "n1", "psc");
    net.connect("inh", "psc", "n1", "psc");

    net.start(100e-3);
    net.saveData();
    
    return 0;
}


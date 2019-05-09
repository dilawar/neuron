/***
 *    Description:  NeuronGroup.
 *
 *        Created:  2019-05-09

 *         Author:  Dilawar Singh <dilawars@ncbs.res.in>
 *   Organization:  NCBS Bangalore
 *        License:  MIT License
 */

#ifndef NEURONGROUP_H
#define NEURONGROUP_H

#include <systemc>
#include <vector>
#include <memory>

#include "../include/IAF.h"
#include "../include/global.h"

using namespace std;

class NeuronGroup: public sc_module
{
public:
    SC_HAS_PROCESS(NeuronGroup);

    NeuronGroup(sc_module_name path, size_t N
            , double rm, double cm, double Em
            );

    string path() const;

    int connect(const string& port, network_variant_t tgt, const string& tgtPort);

    void addClock(sc_clock clk);

    template<typename T=IAF>
    T* getNeuron(size_t i)
    {
        return vecNeurons_[i].get();
    }

public:
    // Ports
    sc_in_clk clock {"clock"};
    

private:
    /* data */
    sc_module_name name_;
    std::string path_;
    size_t N_;

    // Use boost::variant when we have more than IAF neurons.
    vector<unique_ptr<IAF>> vecNeurons_;
};



#endif /* end of include guard: NEURONGROUP_H */

/***
 *    Description:  SpikeGenerator
 *
 *         Author:  Dilawar Singh <dilawars@ncbs.res.in>
 *   Organization:  NCBS Bangalore
 *        License:  MIT License
 */

#ifndef SPIKEGENERATOR_BASE_H
#define SPIKEGENERATOR_BASE_H

#include <systemc.h>
#include "../include/global.h"

#include <memory>

using namespace std;

class SpikeGeneratorBase : public sc_module
{
public:
    SC_HAS_PROCESS(SpikeGeneratorBase);

    // Incoming clock.
    sc_in_clk clock;

    SpikeGeneratorBase(sc_module_name name, size_t N, double period = 1);

    // Connect to other objects.
    int connect(const string& port, network_variant_t tgt, const string& tgtPort);

    void generateSpike( );

    virtual void process() = 0;

    string path();

private:
    sc_module_name name_;
    size_t N_;

    std::vector<unique_ptr<sc_signal<bool> > > spike_;

    string path_;
    double dt_; 
};

#endif /* end of include guard: SPIKEGENERATOR_BASE_H */

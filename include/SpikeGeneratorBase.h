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
#include <memory>

using namespace std;

class SpikeGeneratorBase : public sc_module
{
public:
    SC_HAS_PROCESS(SpikeGeneratorBase);

    // Incoming clock.
    sc_in_clk clock;

    SpikeGeneratorBase(sc_module_name name, size_t N, double period = 1);

    void generateSpike( );

    virtual void process() = 0;

private:
    sc_module_name name_;
    size_t N_;

    // std::vector<sc_signal<double>* > out_;
    std::vector<unique_ptr<sc_signal<double> > > out_;

    string path_;
    double dt_; 
};

#endif /* end of include guard: SPIKEGENERATOR_BASE_H */

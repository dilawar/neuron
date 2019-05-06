/***
 *    Description:  SpikeGenerator
 *
 *         Author:  Dilawar Singh <dilawars@ncbs.res.in>
 *   Organization:  NCBS Bangalore
 *        License:  MIT License
 */

#ifndef SPIKEGENERATOR_H
#define SPIKEGENERATOR_H

#include <systemc.h>

using namespace std;

class SpikeGenerator : public sc_module
{
    SC_HAS_PROCESS(SpikeGenerator);
    
    // ports.
    sc_signal<bool> trigger;
    sc_signal<bool> spike;

public:
    SpikeGenerator(sc_module_name name, double period);

    void generateSpike( );

private:
    sc_module_name name_;
    string path_;
    double dt_; 
};

#endif /* end of include guard: SPIKEGENERATOR_H */
/***
 *    Description:  Spike Generator.

 *         Author:  Dilawar Singh <dilawars@ncbs.res.in>
 *   Organization:  NCBS Bangalore
 *        License:  MIT License
 */

#include "SpikeGenerator.h"

SpikeGenerator::SpikeGenerator(sc_module_name name, double dt):
    name_(name)
    dt_(dt)
{
    path_ = string( (const char*) name );

    SC_METHOD(generate_spike);
    sensitive << trigger;

    out.write(false);
}

void SpikeGenerator::generateSpike( )
{
    wait(dt_-1e-6, SC_SEC);
    out.write(true);
    wait(1e-6, SC_SEC);
    out.write(false);
}

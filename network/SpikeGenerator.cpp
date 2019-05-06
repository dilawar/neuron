/***
 *    Description:  Spike Generator.

 *         Author:  Dilawar Singh <dilawars@ncbs.res.in>
 *   Organization:  NCBS Bangalore
 *        License:  MIT License
 */

#include "../include/SpikeGenerator.h"

SpikeGenerator::SpikeGenerator(sc_module_name name, double dt):
    name_(name)
    , dt_(dt)
{
    path_ = string( (const char*) name );

    SC_METHOD(generateSpike);
    sensitive << trigger;

    spike.write(false);
}

void SpikeGenerator::generateSpike( )
{
    wait(dt_-1e-6, SC_SEC);
    spike.write(true);
    wait(1e-6, SC_SEC);
    spike.write(false);
}

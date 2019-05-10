/***
 *    Description:  Periodic spike generator.
 *
 *         Author:  Dilawar Singh <dilawars@ncbs.res.in>
 *   Organization:  NCBS Bangalore
 *        License:  MIT License
 */

#include "../include/PeriodicSpikeGenerator.h"
#include "spdlog/spdlog.h"

PeriodicSpikeGenerator::PeriodicSpikeGenerator(sc_module_name name, size_t N, double period)
    : SpikeGeneratorBase((const char*)name, N, period)
{
    
}

void PeriodicSpikeGenerator::process()
{}

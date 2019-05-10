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
    : SpikeGeneratorBase((const char*)name, N)
      , period_(sc_time(period, SC_SEC))
{
    spdlog::debug( "++ SpikeGenerator has timeperiod of {} s", period_.to_seconds() );

    SC_THREAD(generateSpike);
}

void PeriodicSpikeGenerator::process()
{

}

void PeriodicSpikeGenerator::generateSpike()
{
    wait(delay_ + globalDt_);
    while(true)
    {
        wait(period_ - globalDt_);
        for (size_t i = 0; i < spike_.size(); i++) 
            spike_[i]->write(true);

        wait(globalDt_);
        for (size_t i = 0; i < spike_.size(); i++) 
            spike_[i]->write(false);
    }
}


/***
 *    Description:  Periodic spike generator.
 *
 *        Created:  2019-05-07

 *         Author:  Dilawar Singh <dilawars@ncbs.res.in>
 *   Organization:  NCBS Bangalore
 *        License:  MIT License
 */

#ifndef PERIODICSPIKEGENERATOR_H
#define PERIODICSPIKEGENERATOR_H

#include "SpikeGeneratorBase.h"

class PeriodicSpikeGenerator: sc_module, public SpikeGeneratorBase
{
public:
    SC_HAS_PROCESS(PeriodicSpikeGenerator);
    PeriodicSpikeGenerator(sc_module_name name, size_t N, double period=1e-3);

    /* data */
    void process( ) override;
    void generateSpike();

private:
    sc_time period_;

};


#endif /* end of include guard: PERIODICSPIKEGENERATOR_H */


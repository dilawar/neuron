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

class PeriodicSpikeGenerator: public SpikeGeneratorBase
{

public:
    SC_HAS_PROCESS(PeriodicSpikeGenerator);
    PeriodicSpikeGenerator(sc_module_name name, size_t N);

    void process();

private:
    /* data */
};


#endif /* end of include guard: PERIODICSPIKEGENERATOR_H */


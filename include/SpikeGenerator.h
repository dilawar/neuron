/***
 *    Description:  Commong header.
 *
 *        Created:  2019-05-07

 *         Author:  Dilawar Singh <dilawars@ncbs.res.in>
 *   Organization:  NCBS Bangalore
 *        License:  MIT License
 */

#ifndef SPIKEGENERATOR_H
#define SPIKEGENERATOR_H

#include "PeriodicSpikeGenerator.h"

class SpikeGenerator
{
public:
    SpikeGenerator()
    {
    }


private:
    /* data */
    unique_ptr<SpikeGeneratorBase> ref_;
};


#endif /* end of include guard: SPIKEGENERATOR_H */

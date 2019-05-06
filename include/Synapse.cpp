/***
 *    Description:  Helper class
 *
 *         Author:  Dilawar Singh <dilawars@ncbs.res.in>
 *   Organization:  NCBS Bangalore
 *        License:  MIT License
 */

#include "../include/Synapse.h"

Synapse::Synapse()
{}

Synapse::~Synapse()
{}

Synapse::Synapse(const string& path, double gbar, double tau, double Em, const string type)
{
    if(type == "alpha")
        syn_ = std::make_unique( new SynapseAlpha(path, gbar, tau, Em));
    else
        syn_ = std::make_unique( new SynapseExp(path, gbar, tau, Em));
}

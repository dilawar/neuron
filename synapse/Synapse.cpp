/***
 *    Description:  Helper class
 *
 *         Author:  Dilawar Singh <dilawars@ncbs.res.in>
 *   Organization:  NCBS Bangalore
 *        License:  MIT License
 */

#include "../include/Synapse.h"

Synapse::Synapse( const string path, const string type)
{
    SynapseBase* baseSyn = nullptr;
    if(type == "alpha")
        baseSyn = new SynapseAlpha(path.c_str());
    else
        baseSyn = new SynapseExp(path.c_str());

    // Assing to unique ptr.
    assert(baseSyn);
    syn_.reset(baseSyn);
}

Synapse::~Synapse()
{}

Synapse::Synapse(const string path, double gbar, double tau, double Em, const string type)
{
    SynapseBase* baseSyn = nullptr;
    if(type == "alpha")
        baseSyn = new SynapseAlpha(path.c_str(), gbar, tau, Em);
    else
        baseSyn = new SynapseExp(path.c_str(), gbar, tau, Em);

    // Assing to unique ptr.
    assert(baseSyn);
    syn_.reset(baseSyn);
}

SynapseBase* Synapse::get() const
{
    return syn_.get();
}

/***
 *    Description:  Helper class
 *
 *         Author:  Dilawar Singh <dilawars@ncbs.res.in>
 *   Organization:  NCBS Bangalore
 *        License:  MIT License
 */

#include "../include/global.h"
#include "../include/SynapseGroup.h"

SynapseGroup::SynapseGroup(sc_module_name path, size_t N
        , double gbar, double tau, double Em, const string type) :
    path_(string((const char*)path))
    , type_(type)
    , N_(N)
{
    for (size_t i = 0; i < N; i++) 
    {
        string synPath = (boost::format("%1%[%2%]")%path%i).str();
        if(type == "alpha")
            elements_.push_back(make_unique<SynapseAlpha>(synPath.c_str(), gbar, tau, Em));
        else
            elements_.push_back(make_unique<SynapseExp>(synPath.c_str(), gbar, tau, Em));
    }
}

SynapseBase* SynapseGroup::getSynapse(size_t i) const
{
    return elements_[i].get();
}

string SynapseGroup::path( )
{
    return path_;
}

string SynapseGroup::type()
{
    return type_;
}

size_t SynapseGroup::size()
{
    return N_;
}

int SynapseGroup::connect(const string& port, network_variant_t tgt, const string& tgtPort)
{
    spdlog::error("Not implemented.");
}

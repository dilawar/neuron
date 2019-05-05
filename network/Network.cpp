/***
 *    Description:  Network.
 *
 *         Author:  Dilawar Singh <dilawars@ncbs.res.in>
 *   Organization:  NCBS Bangalore
 *        License:  MIT License
 */

#include "../include/Synapse.h"
#include "../include/Network.h"

Network::Network()
{}

Network::Network(const string& path): 
    path_(path)
{}

Network::~Network()
{}

SynapseAlpha* Network::addSynapseAlpha(const string& path)
{
    auto syn = new SynapseAlpha(path.c_str());
    elements_["synapse.alpha"].push_back(boost::any(syn));
    return syn;
}

SynapseExp* Network::addSynapseExp(const string& path)
{
    auto syn = new SynapseExp(path.c_str());
    elements_["synapse.exp"].push_back(boost::any(syn));
    return syn;
}

string Network::path() const
{
    return path_;
}

vector<boost::any> Network::getSynapses()
{
    return elements_["synapse"];
}

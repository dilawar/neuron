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

// Alpha synapses.
void Network::addSynapseAlpha(const string& path)
{
    auto syn = new SynapseAlpha(path.c_str());
    elements_["synapse.alpha"].push_back(boost::any(syn));
}

void Network::addSynapseAlpha(const SynapseAlpha* pSyn)
{
    elements_["synapse.alpha"].push_back(boost::any(pSyn));
}

// Exp synapses.
void Network::addSynapseExp(const string& path)
{
    auto syn = new SynapseExp(path.c_str());
    elements_["synapse.exp"].push_back(boost::any(syn));
}

void Network::addSynapseExp(const SynapseExp* pSyn)
{
    elements_["synapse.exp"].push_back(boost::any(pSyn));
}

string Network::path() const
{
    return path_;
}

vector<boost::any> Network::getSynapses( )
{
    return elements_["synapse.alpha"];
}

vector<boost::any> Network::getSynapses(const string ctype)
{
    return elements_["synapse." + ctype];
}

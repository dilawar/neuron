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

void Network::addSynapse(const string& path, const string& type)
{
    boost::any syn(nullptr);
    if(type == "alpha")
        syn = new SynapseAlpha(path.c_str());
    else
        syn = new SynapseExp(path.c_str());

    std::cout << "Info: " << "Created synapse " << path << std::endl;
    elements_[path] = syn;
}

string Network::path() const
{
    return path_;
}

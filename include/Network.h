/***
 *    Description:  Network.
 *
 *        Created:  2019-05-05

 *         Author:  Dilawar Singh <dilawars@ncbs.res.in>
 *   Organization:  NCBS Bangalore
 *        License:  MIT License
 */

#ifndef NETWORK_H
#define NETWORK_H

#include <memory>
#include <vector>
#include <map>
#include <boost/any.hpp>

using namespace std;

class Network
{
    public:
        Network();
        Network(const string path);
        ~Network();

        void addSynapseAlpha(const string path);
        void addSynapseExp(const string path);

        void addSynapseAlpha(const SynapseAlpha* ptr);
        void addSynapseExp(const SynapseExp* ptr);

        void addNeuron(const string path, const string& type="iaf");

        vector<boost::any> getSynapses( );
        vector<boost::any> getSynapses(const string ctype);

        string path() const;

    private:
        string path_;
        /* data */
        map<string, vector<boost::any> > elements_;
};



#endif /* end of include guard: NETWORK_H */

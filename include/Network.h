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
        Network(const string& path);
        ~Network();

        void addSynapse(const string& path, const string& type="alpha");
        void addNeuron(const string& path, const string& type="iaf");

        string path() const;

    private:
        string path_;
        /* data */
        map<string, boost::any> elements_;
};



#endif /* end of include guard: NETWORK_H */

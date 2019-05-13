// =====================================================================================
//
//       Filename:  Synapse.h
//
//    Description:  Include all other synapses.
//
//        Version:  1.0
//        Created:  Friday 03 May 2019 04:50:43  IST
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Dilawar Singh (), dilawar.s.rajput@gmail.com
//   Organization:  NCBS Bangalore
//
// =====================================================================================

#ifndef SYNAPSE_H
#define SYNAPSE_H

#include "global.h"
#include "SynapseExp.h"
#include "SynapseAlpha.h"

#include <memory>
#include <vector>

// Forward declaration
class Network;

using namespace std;


/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  Helper class to wrap various types of synapses. Also used in
 */
/* ----------------------------------------------------------------------------*/
class SynapseGroup : public sc_module
{
public:
    SC_HAS_PROCESS(SynapseGroup);

    SynapseGroup(sc_module_name path, size_t N
                 , double gbar
                 , double tau
                 , double Em
                 , const string type="alpha"
                );

    // Get a particular synapse.
    SynapseBase* getSynapse(size_t i) const;

    // Connector. 
    int connect(const string& port, network_variant_t tgt, const string& tgtPort, Network* net);

    // Accessors.
    string path();
    string type();
    size_t size();
    
    void showGroupInfo() const;

private:
    /* data */
    string path_;
    string type_;
    size_t N_;

    // Keeps all elements.
    vector<unique_ptr<SynapseBase>> elements_;
};



#endif /* end of include guard: SYNAPSE_H */

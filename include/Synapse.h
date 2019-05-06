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

#include "SynapseExp.h"
#include "SynapseAlpha.h"

#include <memory>
#include <vector>

using namespace std;

class SynapseAlpha;
class SynapseExp;

/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  Helper class to wrap various types of synapses. Also used in 
 */
/* ----------------------------------------------------------------------------*/
class Synapse
{
    public:
        Synapse();
        Synapse(const string& path);
        Synapse(const string& path, double gbar, double tau, double Em, const string& type="alpha");
        ~Synapse();

    private:
        /* data */
        string path_;
        string type_;
        unique_ptr<SynapseBase> syn_;
};


#endif /* end of include guard: SYNAPSE_H */

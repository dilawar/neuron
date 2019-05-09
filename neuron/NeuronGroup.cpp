/***
 *    Description:  NeuronGroup class.
 *
 *        Created:  2019-05-09

 *         Author:  Dilawar Singh <dilawars@ncbs.res.in>
 *   Organization:  NCBS Bangalore
 *        License:  MIT License
 */

#include "../include/NeuronGroup.h"
#include "../network/Connectors.hh"

NeuronGroup::NeuronGroup(sc_module_name name, size_t N, double rm, double cm, double Em):
    name_(name)
    , path_((const char*)name)
    , N_(N)
{
    // Now construct the neurons.
    string mPath = string((const char*)name);

    for (size_t i = 0; i < N; i++) 
    {
        string nrnName = (const char*)name + '[' + to_string(i) + ']';
        unique_ptr<IAF> iaf( make_unique<IAF>(nrnName.c_str(), rm, cm, Em) );
        iaf->clock(clock);
        // Now keep it in vector.
        vecNeurons_.push_back( std::move(iaf) );
    }
}


int NeuronGroup::connect(const string& port, network_variant_t tgt, const string& tgtPort)
{
    // See Connections.hh and boost::variant static visitor.
    return boost::apply_visitor(
            std::bind(NeuronGroupConnectionVisitor(), this, port, std::placeholders::_1, tgtPort)
            , tgt);
}

string NeuronGroup::path() const
{
    return path_;
}


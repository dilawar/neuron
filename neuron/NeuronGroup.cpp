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

#include "spdlog/spdlog.h"

NeuronGroup::NeuronGroup(sc_module_name name, size_t N, double rm, double cm, double Em):
    name_(name)
    , path_((const char*)name)
    , N_(N)
{
    // Now construct the neurons.
    string mPath = string((const char*)name);
    for (size_t i = 0; i < N; i++) 
    {
        unique_ptr<IAF> iaf( make_unique<IAF>(
                    (boost::format("%1%[%2%]")%name%i).str().c_str(), rm, cm, Em) 
                );

        spdlog::debug( "++ Created neuron {}", iaf->repr() );

        // Now keep it in vector.
        vecNeurons_.push_back( std::move(iaf) );
        // Port binding is done by Network 
    }
}

int NeuronGroup::connect(const string& port, network_variant_t tgt, const string& tgtPort, Network* net)
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

size_t NeuronGroup::size() const 
{
    return N_;
}


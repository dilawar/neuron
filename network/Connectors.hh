/***
 *    Description:  Connect source and target using boost::variant static
 *    visitors.
 *
 *        Created:  2019-05-09

 *         Author:  Dilawar Singh <dilawars@ncbs.res.in>
 *   Organization:  NCBS Bangalore
 *        License:  MIT License
 */

#ifndef CONNECTORS_H
#define CONNECTORS_H

#include "../include/tantrika.h"
#include "../utility/sc_utils.hpp"

// This is visitor for binding port.  
class NetworkPortBinderVisitor : public boost::static_visitor<int>
{
public:
    int operator()(SpikeGeneratorBase* ptr, Network* net) const
    {
        spdlog::debug( "+ Binding port of SpikeGeneratorBase {}", ptr->path());
        return 0;
    }
    
    int operator()(NeuronGroup* ptr, Network* net) const
    {
        spdlog::debug( "+ Binding port of NeuronGroup {}", ptr->path());
        return 0;
    }

    int operator()(SynapseGroup* ptr, Network* net) const
    {
        spdlog::debug( "+ Binding port of SynapseGroup {}", ptr->path());
        return 1;
    }
};

// This is visitor for connecting 
class NetworkConnectionVisitor : public boost::static_visitor<int>
{
public:
    int operator()(SpikeGeneratorBase* ptr, const string& port
            , network_variant_t tgt , const string tgtPortName) const
    {
        spdlog::debug( "+ SpikeGenerator connect .{} to .{}", port, tgtPortName);
        return 1;
    }
    
    int operator()(NeuronGroup* ptr, const string& port
            , network_variant_t tgt , const string tgtPortName) const
    {
        spdlog::debug( "+ NeuronGroup connect .{} to .{}", port, tgtPortName);
        return 1;
    }

    int operator()(SynapseGroup* ptr, const string& port
            , network_variant_t tgt , const string tgtPortName) const
    {
        spdlog::debug( "+ SynapseGroup connect .{} to .{}", port, tgtPortName);
        return 1;
    }
};



// This is visitor for connecting 
class NetworkConnectionVisitorOld : public boost::static_visitor<int>
{
public:
    int operator()(SpikeGeneratorBase* ptr, const string& port
            , network_variant_t tgt , const string tgtPortName) const
    {
        spdlog::debug( "+ SpikeGenerator connect .{} to .{}", port, tgtPortName);
        return ptr->connect(port, tgt, tgtPortName);
    }
    
    int operator()(NeuronGroup* ptr, const string& port
            , network_variant_t tgt , const string tgtPortName) const
    {
        spdlog::debug( "+ NeuronGroup connect .{} to .{}", port, tgtPortName);
        return ptr->connect(port, tgt, tgtPortName);
    }

    int operator()(SynapseGroup* ptr, const string& port
            , network_variant_t tgt , const string tgtPortName) const
    {
        spdlog::debug( "+ SynapseGroup connect .{} to .{}", port, tgtPortName);
        return ptr->connect(port, tgt, tgtPortName);
    }
};

class SpikeGneneratorBaseConnectionVisitor: public boost::static_visitor<int>
{
public:
    int operator()(SpikeGeneratorBase* ptr, const string& port
            , const SpikeGeneratorBase* tgt, const string tgtPortName) const
    {
        spdlog::error( "+ SpikeGeneratorBase to SpikeGeneratorBase is not supported..");
        return -1;
    }
    
    int operator()(SpikeGeneratorBase* ptr, const string& port
            , const NeuronGroup* tgt , const string tgtPortName) const
    {
        spdlog::error( "+ SpikeGeneratorBase to NeuronGroup is not implemented yet..");
        return -1;
    }

    int operator()(SpikeGeneratorBase* ptr, const string& port
            , SynapseGroup* syns, string tgtPort) const
    {
        for (size_t i = 0; i < syns->size(); i++) 
        {
            // find target port.
            auto syn = syns->getSynapse(i);
            auto pTgtPort = findPort<sc_in<bool> >(syn, tgtPort, "sc_in");
            if(! pTgtPort)
            {
                spdlog::warn( "Could not find {}.{}. Ignoring rest ...", syn->name(), pTgtPort->basename());
                return -1;
            }
            pTgtPort->bind( *(ptr->getSpikePort(i)) );
        }

        spdlog::debug("\t\t ... SUCCESS.");
        return 0;
    }
};

// NeuronGroup visitor.
class NeuronGroupConnectionVisitor: public boost::static_visitor<int>
{
public:
    int operator()(NeuronGroup* srcGroup, const string& srcPort
            , const SpikeGeneratorBase* tgtGroup, const string tgtPortName) const
    {
        spdlog::error( "+ NeuronGroup to SpikeGeneratorBase is not supported..");
        return -1;
    }
    
    int operator()(NeuronGroup* srcGroup, const string& srcPort
            , const NeuronGroup* tgtGroup, const string tgtPortName) const
    {
        spdlog::error( "+ NeuronGroup to NeuronGroup is not implemented yet..");
        return -1;
    }

    int operator()(NeuronGroup* srcGroup, const string& srcPort
            , SynapseGroup* synGroup, string tgtPort) const
    {
        // Find the way to find the object
        for (size_t i = 0; i < synGroup->size(); i++) 
        {
            // find target port.
            auto src = srcGroup->getNeuron(i);
            auto pSrcPort = findPort<sc_out<double> >(src, srcPort);
            if(! pSrcPort)
            {
                spdlog::warn( "Could not find {}.{}. Available: {}. Ignoring rest.", src->path()
                        , srcPort, availablePortsCSV(src)
                        );
                return -1;
            }

            auto tgt = synGroup->getSynapse(i);
            sc_in<double>* pTgtPort = findPort<sc_in<double> >(tgt, tgtPort);
            if(! pTgtPort)
            {
                spdlog::warn( "Could not find {}.{}. Availble: {}.", tgt->path(), tgtPort
                        , availablePortsCSV(tgt)
                        );
                return -1;
            }

            pTgtPort->bind(*pSrcPort);
            spdlog::debug("+++ Bound {} to {}", pTgtPort->name(), pSrcPort->name());
            // (*pTgtPort)(*pSrcPort);
        }

        spdlog::debug("\t\t ... SUCCESS.");
        return 0;
    }
};


#endif /* end of include guard: CONNECTORS_H */

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

template<typename T=double>
string signalForConnectingPorts(sc_out<T>* out, sc_in<T>* in)
{
    return sanitizePath((boost::format("%1%##%2%")%in->name()%out->name()).str());
}

template<typename T=double>
unique_ptr<sc_signal<T>> connectPorts(sc_out<T>* out, sc_in<T>* in) {
    string sigName = signalForConnectingPorts(out, in);
    unique_ptr<sc_signal<T>> sig = make_unique<sc_signal<T>>(sigName.c_str(), (T)0);
    in->bind(*sig);
    out->bind(*sig);
    return std::move(sig);
}

/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  This class is the top-level class to bind ports.
 */
/* ----------------------------------------------------------------------------*/
class NetworkConnectionVisitor : public boost::static_visitor<int> {
public:
    int operator()(SpikeGeneratorBase* ptr, const string& port
                   , network_variant_t tgt, const string tgtPortName, Network* net) const {
        spdlog::debug( "+ SpikeGenerator connect .{} to .{}", port, tgtPortName);
        return ptr->connect(port, tgt, tgtPortName, net);
    }

    int operator()(NeuronGroup* ptr, const string& port
                   , network_variant_t tgt, const string tgtPortName, Network* net) const {
        spdlog::debug( "+ NeuronGroup connect .{} to .{}", port, tgtPortName);
        return ptr->connect(port, tgt, tgtPortName, net);
    }

    int operator()(SynapseGroup* ptr, const string& port
                   , network_variant_t tgt, const string tgtPortName, Network* net) const {
        spdlog::debug( "+ SynapseGroup connect .{} to .{}", port, tgtPortName);
        return ptr->connect(port, tgt, tgtPortName, net);
    }
};


/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  Bind SynapseGroup port.
 */
/* ----------------------------------------------------------------------------*/
class SynapseGroupConnectionVisitor: public boost::static_visitor<int> {
public:
    int operator()(SynapseGroup* ptr, const string& port
                   , const SpikeGeneratorBase* tgt, const string tgtPortName, Network* net) const {
        spdlog::error( "+ SynapseGroup to SpikeGeneratorBase is not supported..");
        throw TantrikaNotImplemented();
    }

    /* --------------------------------------------------------------------------*/
    /**
     * @Synopsis  Connecting SynapseGroup to NeuronGroup.
     *
     * @Param srcGroup
     * @Param srcPort
     * @Param tgtGroup
     * @Param tgtPort
     * @Param net
     *
     * @Returns
     */
    /* ----------------------------------------------------------------------------*/
    int operator()(SynapseGroup* srcGroup, const string& srcPort
                   , NeuronGroup* tgtGroup, const string tgtPort, Network* net) const {
        // Find the way to find the object. Bind 1 to 1.
        for (size_t i = 0; i < srcGroup->size(); i++) {
            auto src = srcGroup->getSynapse(i);
            auto pSrcPort = findPort<sc_out<double>>(src, srcPort);
            if(! pSrcPort) {
                spdlog::warn( "Could not find {}.{}. Available: {}. Ignoring rest.", src->path()
                              , srcPort, availablePortsCSV(src)
                            );
                return -1;
            }

            auto tgt = tgtGroup->getNeuron(i);

            if( tgtPort != "psc") {
                sc_in<double>* pTgtPort = findPort<sc_in<double> >(tgt, tgtPort);
                if(! pTgtPort) {
                    spdlog::warn( "Could not find {}.{}. Availble: {}.", tgt->path(), tgtPort
                                  , availablePortsCSV(tgt)
                                );
                    return -1;
                } else {
                    connectPorts(pSrcPort, pTgtPort);
                }
            } else {
                // Create an intermediate signal.
                string sigName = sanitizePath(src->path() + ".psc");
                unique_ptr<sc_signal<double>> ps = make_unique<sc_signal<double>>(sigName.c_str());
                pSrcPort->bind(*ps);
                tgt->bindSynapse(ps.get());
                net->addSignal(std::move(ps));
            }
        }

        spdlog::debug("\t\t ... SUCCESS.");
        return 0;

        throw TantrikaNotImplemented();
    }

    int operator()(SynapseGroup* ptr, const string& port
                   , SynapseGroup* syns, string tgtPort, Network* net) const {
        spdlog::debug("\t\t ... SUCCESS.");
        throw TantrikaNotImplemented();
    }
};


// SpikeGenerator
class SpikeGneneratorBaseConnectionVisitor: public boost::static_visitor<int> {
public:
    int operator()(SpikeGeneratorBase* ptr, const string& port
                   , const SpikeGeneratorBase* tgt, const string tgtPortName, Network* net) const {
        spdlog::error( "+ SpikeGeneratorBase to SpikeGeneratorBase is not supported..");
        throw TantrikaNotImplemented();
    }

    int operator()(SpikeGeneratorBase* ptr, const string& port
                   , const NeuronGroup* tgt, const string tgtPortName, Network* net) const {
        spdlog::error( "+ SpikeGeneratorBase to NeuronGroup is not implemented yet..");
        throw TantrikaNotImplemented();
    }

    int operator()(SpikeGeneratorBase* ptr, const string& port
                   , SynapseGroup* syns, string tgtPort, Network* net) const {
        for (size_t i = 0; i < syns->size(); i++) {
            // find target port.
            auto syn = syns->getSynapse(i);
            auto pTgtPort = findPort<sc_in<bool> >(syn, tgtPort, "sc_in");
            if(! pTgtPort) {
                spdlog::warn( "Could not find {}.{}. Ignoring rest ...", syn->name(), pTgtPort->basename());
                return -1;
            }
            auto pSrcPort = ptr->getSpikePort(i);
            spdlog::debug( "++ Connecting {} and {}", pTgtPort->name(), pSrcPort->name());

            // Create an intermediate signal.
            auto pSig = connectPorts<bool>(pSrcPort, pTgtPort);
            net->addSignal(std::move(pSig));
        }
        spdlog::debug("\t\t ... SUCCESS.");
        return 0;
    }
};

// NeuronGroup visitor.
class NeuronGroupConnectionVisitor: public boost::static_visitor<int> {
public:
    int operator()(NeuronGroup* srcGroup, const string& srcPort
                   , const SpikeGeneratorBase* tgtGroup, const string tgtPortName) const {
        spdlog::error( "+ NeuronGroup to SpikeGeneratorBase is not supported..");
        throw TantrikaNotImplemented();
    }

    int operator()(NeuronGroup* srcGroup, const string& srcPort
                   , const NeuronGroup* tgtGroup, const string tgtPortName) const {
        spdlog::error( "+ NeuronGroup to NeuronGroup is not implemented yet..");
        throw TantrikaNotImplemented();
    }

    int operator()(NeuronGroup* srcGroup, const string& srcPort
                   , SynapseGroup* synGroup, string tgtPort) const {
        // Find the way to find the object
        for (size_t i = 0; i < synGroup->size(); i++) {
            // find target port.
            auto src = srcGroup->getNeuron(i);
            auto pSrcPort = findPort<sc_out<double>>(src, srcPort);
            if(! pSrcPort) {
                spdlog::warn( "Could not find {}.{}. Available: {}. Ignoring rest.", src->path()
                              , srcPort, availablePortsCSV(src)
                            );
                return -1;
            }

            auto tgt = synGroup->getSynapse(i);
            sc_in<double>* pTgtPort = findPort<sc_in<double> >(tgt, tgtPort);
            if(! pTgtPort) {
                spdlog::warn( "Could not find {}.{}. Availble: {}.", tgt->path(), tgtPort
                              , availablePortsCSV(tgt)
                            );
                return -1;
            }

            pTgtPort->bind(*pSrcPort);
            spdlog::debug("+++ Bound {} to {}", pTgtPort->name(), pSrcPort->name());
        }

        spdlog::debug("\t\t ... SUCCESS.");
        return 0;
    }
};

/* --------------------------------------------------------------------------*/
/**
 * @Synopsis Port binder. This is the top-level visitor called by Network to
 * bind ports with signals in Network which are yet not bound by user.
 *
 * This way we can do the simulation.
 */
/* ----------------------------------------------------------------------------*/
class NetworkPortBinderVisitor : public boost::static_visitor<int> {
public:
    int operator()(SpikeGeneratorBase* ptr, Network* net) const {
        spdlog::debug( "+ Binding unbound ports of SpikeGeneratorBase {}", ptr->path());

        // Find ports which are not bound and bind them to Network::signals.
        for (size_t i = 0; i < ptr->size(); i++) {
            sc_out<bool>* sPort = ptr->getSpikePort(i);
            net->connectPort<bool, sc_out<bool>>(sPort);
        }
        return 0;
    }

    int operator()(NeuronGroup* ptr, Network* net) const {
        spdlog::debug( "+ Binding unbound ports of NeuronGroup {}", ptr->path());

        for (size_t i = 0; i < ptr->size(); i++) {
            IAF* n = ptr->getNeuron(i);

            // List ports manually.
            net->connectPort<double, sc_out<double>>(&n->vm);
            net->connectPort<double, sc_in<double>>(&n->inject);
        }
        return 0;
    }

    int operator()(SynapseGroup* ptr, Network* net) const {
        spdlog::debug( "+ Binding unbound ports of SynapseGroup {}", ptr->path());

        for (size_t i = 0; i < ptr->size(); i++) {
            SynapseBase* n = ptr->getSynapse(i);

            // List ports manually.
            net->connectPort<bool, sc_in<bool>>(&n->spike);
            net->connectPort<double, sc_in<double>>(&n->post);
            net->connectPort<double, sc_out<double>>(&n->psc);
        }

        return 0;
    }
};



#endif /* end of include guard: CONNECTORS_H */

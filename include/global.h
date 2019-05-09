#ifndef GLOBAL_H
#define GLOBAL_H

#include "spdlog/spdlog.h"
#include <boost/variant.hpp>
#include <boost/format.hpp>

class SynapseGroup;
class SpikeGeneratorBase;
class NeuronGroup;

typedef boost::variant<
            SynapseGroup*
            , SpikeGeneratorBase*   // SpikeGeneratorBase.h
            , NeuronGroup*          // NeuronGroup.h
        > network_variant_t;

#endif /* end of include guard: GLOBAL_H */

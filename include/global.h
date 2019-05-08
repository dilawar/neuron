#ifndef GLOBAL_H
#define GLOBAL_H

#include<boost/variant.hpp>

class SynapseGroup;
class SpikeGeneratorBase;

typedef boost::variant<SynapseGroup*, SpikeGeneratorBase*> network_variant_t;

#endif /* end of include guard: GLOBAL_H */

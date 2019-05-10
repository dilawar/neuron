/***
 *    Description:  Spike Generator.

 *         Author:  Dilawar Singh <dilawars@ncbs.res.in>
 *   Organization:  NCBS Bangalore
 *        License:  MIT License
 */

#include "../include/SpikeGenerator.h"
#include "../include/SynapseGroup.h"
#include "../network/Connectors.hh"

SpikeGeneratorBase::SpikeGeneratorBase(const string& path, size_t N)
    : path_(path)
    , N_(N)
{

    for (size_t i = 0; i < N_; i++) 
        spike_.push_back( sc_out2<bool>((boost::format("spike[%1%]")%i).str().c_str()) );
}

int SpikeGeneratorBase::connect(const string& port, network_variant_t tgt, const string& tgtPort, Network* net)
{
    return boost::apply_visitor(
            std::bind(SpikeGneneratorBaseConnectionVisitor(), this, port, std::placeholders::_1, tgtPort, net)
            , tgt);
}

sc_out2<bool> SpikeGeneratorBase::getSpikePort(size_t i) const
{
    return spike_[i];
}

string SpikeGeneratorBase::path() const
{
    return path_;
}


size_t SpikeGeneratorBase::size() const
{
    return spike_.size();
}

void SpikeGeneratorBase::setDelay(double delay)
{
    delay_ = sc_time(delay, SC_SEC);
}

double SpikeGeneratorBase::getDelay(void) const
{
    return delay_.to_seconds();
}

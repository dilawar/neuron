/***
 *    Description:  Spike Generator.

 *         Author:  Dilawar Singh <dilawars@ncbs.res.in>
 *   Organization:  NCBS Bangalore
 *        License:  MIT License
 */

#include "../include/SpikeGenerator.h"
#include "../include/SynapseGroup.h"
#include "../network/Connectors.hh"

SpikeGeneratorBase::SpikeGeneratorBase(sc_module_name name, size_t N, double dt)
    : path_((const char*)name)
    , N_(N)
    , dt_(dt)
{
    for (size_t i = 0; i < N_; i++) 
        spike_.push_back( make_unique<sc_out<bool> >() );

    SC_THREAD(generateSpike);
}

void SpikeGeneratorBase::generateSpike( )
{
    while(true)
    {
        wait(dt_-1e-6, SC_SEC);
        for (size_t i = 0; i < spike_.size(); i++) 
            spike_[i]->write(true);

        wait(1e-6, SC_SEC);
        for (size_t i = 0; i < spike_.size(); i++) 
            spike_[i]->write(false);
    }
}

int SpikeGeneratorBase::connect(const string& port, network_variant_t tgt, const string& tgtPort)
{
    return boost::apply_visitor(
            std::bind(SpikeGneneratorBaseConnectionVisitor(), this, port, std::placeholders::_1, tgtPort)
            , tgt);
}

sc_out<bool>* SpikeGeneratorBase::getSpikePort(size_t i)
{
    return spike_[i].get();
}

string SpikeGeneratorBase::path() const
{
    return path_;
}


size_t SpikeGeneratorBase::size() const
{
    return spike_.size();
}

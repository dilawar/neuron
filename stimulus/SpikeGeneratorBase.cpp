/***
 *    Description:  Spike Generator.

 *         Author:  Dilawar Singh <dilawars@ncbs.res.in>
 *   Organization:  NCBS Bangalore
 *        License:  MIT License
 */

#include "../include/SpikeGenerator.h"
#include "../include/SynapseGroup.h"
#include "../network/Connectors.hh"

SpikeGeneratorBase::SpikeGeneratorBase(sc_module_name name, size_t N, double dt):
    name_(name)
    , N_(N)
    , dt_(dt)
{
    path_ = string( (const char*) name );

    for (size_t i = 0; i < N_; i++) 
    {
        string sname = "out"+to_string(i);
        spike_.push_back( make_unique<sc_signal<bool> >(sname.c_str(), false) );
    }

    SC_METHOD(generateSpike);
    sensitive << clock;
}

void SpikeGeneratorBase::generateSpike( )
{
    wait(dt_-1e-6, SC_SEC);
    for (size_t i = 0; i < spike_.size(); i++) 
        spike_[i]->write(true);

    wait(1e-6, SC_SEC);
    for (size_t i = 0; i < spike_.size(); i++) 
        spike_[i]->write(false);
}

int SpikeGeneratorBase::connect(const string& port, network_variant_t tgt, const string& tgtPort)
{
    return boost::apply_visitor(
            std::bind(SpikeGneneratorBaseConnectionVisitor(), this, port, std::placeholders::_1, tgtPort)
            , tgt);
}

sc_signal<bool>* SpikeGeneratorBase::getSpikePort(size_t i)
{
    return spike_[i].get();
}

string SpikeGeneratorBase::path()
{
    return path_;
}

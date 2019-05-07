/***
 *    Description:  Spike Generator.

 *         Author:  Dilawar Singh <dilawars@ncbs.res.in>
 *   Organization:  NCBS Bangalore
 *        License:  MIT License
 */

#include "../include/SpikeGenerator.h"

SpikeGeneratorBase::SpikeGeneratorBase(sc_module_name name, size_t N, double dt):
    name_(name)
    , N_(N)
    , dt_(dt)
{
    path_ = string( (const char*) name );
    for (size_t i = 0; i < N_; i++) 
    {
        string sname = "out"+to_string(i);
        // out_.push_back( new sc_signal<double>(sname.c_str(), 0.0) );
        out_.push_back( make_unique<sc_signal<double> >(sname.c_str(), 0.0) );
    }

    SC_METHOD(generateSpike);
    sensitive << clock;
}

void SpikeGeneratorBase::generateSpike( )
{
    wait(dt_-1e-6, SC_SEC);
    for (size_t i = 0; i < out_.size(); i++) 
        out_[i]->write(true);

    wait(1e-6, SC_SEC);
    for (size_t i = 0; i < out_.size(); i++) 
        out_[i]->write(false);
}

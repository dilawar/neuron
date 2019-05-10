/***
 *    Description:  SpikeGenerator
 *
 *         Author:  Dilawar Singh <dilawars@ncbs.res.in>
 *   Organization:  NCBS Bangalore
 *        License:  MIT License
 */

#ifndef SPIKEGENERATOR_BASE_H
#define SPIKEGENERATOR_BASE_H

#include <systemc.h>
#include "../include/global.h"

#include <memory>

using namespace std;

class SpikeGeneratorBase 
{
public:
    SpikeGeneratorBase(const string& name, size_t N);

    // Connect to other objects.
    int connect(const string& port, network_variant_t tgt, const string& tgtPort);

    virtual void process() = 0;

    // setter
    void setDelay(double delay);

    // Getter
    sc_out<bool>* getSpikePort(size_t i) const;
    double getDelay( ) const;

    string path() const;

    size_t size() const;

public:
    // Incoming clock.
    sc_in_clk clock{"clock"};
    std::vector<unique_ptr<sc_out<bool> > > spike_;

protected:
    string path_;
    size_t N_;
    sc_time dt_; 
    sc_time delay_;

};

#endif /* end of include guard: SPIKEGENERATOR_BASE_H */

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

// Forward declaration
class Network;


class SpikeGeneratorBase 
{
public:
    SpikeGeneratorBase(const string& name, size_t N);

    // Connect to other objects.
    int connect(const string& port, network_variant_t tgt, const string& tgtPort, Network* net);

    virtual void process() = 0;

    // setter
    void setDelay(double delay);

    // Getter
    sc_out<bool>* getSpikePort(size_t i) const;
    double getDelay( ) const;

    string path() const;

    size_t size() const;

    // Helper
    void showGroupInfo() const;

public:
    // template<typename T>
    // using sc_out_multi_multi_p<T, N> = sc_port<sc_signal_out_if<T>, 10, SC_ZERO_OR_MORE_BOUND>;

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

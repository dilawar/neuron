#ifndef GLOBAL_H
#define GLOBAL_H

#include "spdlog/spdlog.h"
#include <boost/variant.hpp>
#include <boost/format.hpp>
#include <systemc>
#include <exception>

class SynapseGroup;
class SpikeGeneratorBase;
class NeuronGroup;

extern sc_core::sc_time globalDt_;

typedef boost::variant<
            SynapseGroup*
            , SpikeGeneratorBase*   // SpikeGeneratorBase.h
            , NeuronGroup*          // NeuronGroup.h
        > network_variant_t;

typedef boost::variant<sc_core::sc_signal<bool>, sc_core::sc_signal<double>> signal_variant_t;

// My ports.
template<typename T>
using sc_in0 = sc_core::sc_port<sc_core::sc_signal_in_if<T>, 1, sc_core::SC_ZERO_OR_MORE_BOUND>;
template<typename T>
using sc_inout0 = sc_core::sc_port<sc_core::sc_signal_inout_if<T>, 1, sc_core::SC_ZERO_OR_MORE_BOUND>;
template<typename T>
using sc_out0 = sc_core::sc_port<sc_core::sc_signal_inout_if<T>, 1, sc_core::SC_ZERO_OR_MORE_BOUND>;

template<typename T>
using sc_in2 = sc_core::sc_port<sc_core::sc_signal_in_if<T>, 2, sc_core::SC_ONE_OR_MORE_BOUND>;
template<typename T>
using sc_inout2 = sc_core::sc_port<sc_core::sc_signal_inout_if<T>, 2, sc_core::SC_ONE_OR_MORE_BOUND>;
template<typename T>
using sc_out2 = sc_core::sc_port<sc_core::sc_signal_inout_if<T>, 2, sc_core::SC_ONE_OR_MORE_BOUND>;

// Not implemented.
class NotImplemented : public std::logic_error
{
private:

    std::string _text;

    NotImplemented(const char* message, const char* function)
        :
        std::logic_error("Not Implemented")
    {
        _text = message;
        _text += " : ";
        _text += function;
    };

public:

    NotImplemented(): NotImplemented("Not Implememented", __FUNCTION__)
    {
    }

    NotImplemented(const char* message): NotImplemented(message, __FUNCTION__)
    {
    }

    const char *what() const throw( )
    {
        return _text.c_str();
    }
};

#endif /* end of include guard: GLOBAL_H */

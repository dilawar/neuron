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

// Extract filename.
constexpr auto* getFileName(const char* const path)
{
    const auto* startPosition = path;
    for (const auto* currentCharacter = path;*currentCharacter != '\0'; ++currentCharacter)
    {
        if (*currentCharacter == '\\' || *currentCharacter == '/')
        {
            startPosition = currentCharacter;
        }
    }

    if (startPosition != path)
    {
        ++startPosition;
    }

    return startPosition;
}

#define TantrikaNotImplemented() \
    std::logic_error( (boost::format("Not implemented: %1%:%2%")%getFileName(__FILE__)%__FUNCTION__).str() );


    

#if 0
namespace tantrika {
// Not implemented.
class NotImplemented : public std::logic_error
{
private:

    std::string _text;

    NotImplemented(const char* message, const char* function) :
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
}

#endif


#endif /* end of include guard: GLOBAL_H */

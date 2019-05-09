// =====================================================================================
//
//       Filename:  sc_ports.hpp
//
//    Description:  User defined ports for tantrika.
//
//        Version:  1.0
//        Created:  09/05/19 06:59:29 PM IST
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Dilawar Singh (), dilawar.s.rajput@gmail.com
//   Organization:  NCBS Bangalore
//
// =====================================================================================

#ifndef SC_PORTS_H
#define SC_PORTS_H

#include <systemc>

template<typename T=double>
using sc_port_out_optional = sc_port<sc_signal_inout_if<T>, 1, SC_ZERO_OR_MORE_BOUND>;

template<typename T=double>
using sc_port_in_optional = sc_port<sc_signal_in_if<T>, 1, SC_ZERO_OR_MORE_BOUND>;

/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  This port is optional and does not require binding.
 *
 * @tparam T
 */
/* ----------------------------------------------------------------------------*/
template <typename T=double>
class sc_out_optional : public sc_port_out_optional<T>
{

public:
    sc_out_optional(const char* name): sc_port_out_optional<T>(name)
    {}

    void write(T a) 
    {
        this->write(a);
    }

    T read()
    {
        return this->read();
    }

};


template <typename T=double>
class sc_in_optional : public sc_port_in_optional<T>
{

public:
    sc_in_optional(const char* name): sc_port_in_optional<T>(name)
    {}

    void write(T a) 
    {
        this->write(a);
    }

    T read()
    {
        return this->read();
    }

};




#endif /* end of include guard: SC_PORTS_H */

// =====================================================================================
//
//       Filename:  sc_port.hpp
//
//    Description:  Specialized port.
//
//        Version:  1.0
//        Created:  09/05/19 08:21:07 PM IST
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Dilawar Singh (), dilawar.s.rajput@gmail.com
//   Organization:  NCBS Bangalore
//
// =====================================================================================


#ifndef TANTRIKA_PORTS_H
#define TANTRIKA_PORTS_H


#include <systemc>

template <typename T>
class sc_out_opt: public sc_port<sc_signal_inout_if<T>, 1, SC_ZERO_OR_MORE_BOUND>
{
public:

    // typedefs
    typedef T                                          data_type;

    typedef sc_signal_inout_if<data_type>               if_type;
    typedef sc_port<if_type,1,SC_ZERO_OR_MORE_BOUND>    base_type;
    typedef sc_out_opt<data_type>                       this_type;

    typedef sc_signal_in_if<data_type>                  in_if_type;
    typedef sc_port<in_if_type,1,SC_ZERO_OR_MORE_BOUND> in_port_type;
    typedef if_type                                     inout_if_type;
    typedef base_type                                   inout_port_type;


public:

    // constructors

    sc_out_opt()
	: base_type(), m_init_val( 0 ), m_traces( 0 ),
	  m_change_finder_p(0)
	{}

    explicit sc_out_opt( const char* name_ )
	: base_type( name_ ), m_init_val( 0 ), m_traces( 0 ),
	  m_change_finder_p(0)
	{}

    explicit sc_out_opt( inout_if_type& interface_ )
	: base_type( interface_ ), m_init_val( 0 ), m_traces( 0 ),
	  m_change_finder_p(0)
	{}

    sc_out_opt( const char* name_, inout_if_type& interface_ )
	: base_type( name_, interface_ ), m_init_val( 0 ), m_traces( 0 ),
	  m_change_finder_p(0)
	{}

    explicit sc_out_opt( inout_port_type& parent_ )
	: base_type( parent_ ), m_init_val( 0 ), m_traces( 0 ),
	  m_change_finder_p(0)
	{}

    sc_out_opt( const char* name_, inout_port_type& parent_ )
	: base_type( name_, parent_ ), m_init_val( 0 ), m_traces( 0 ),
	  m_change_finder_p(0)
	{}

    sc_out_opt( this_type& parent_ )
	: base_type( parent_ ), m_init_val( 0 ), m_traces( 0 ),
	  m_change_finder_p(0)
	{}

    sc_out_opt( const char* name_, this_type& parent_ )
	: base_type( name_, parent_ ), m_init_val( 0 ), m_traces( 0 ),
	  m_change_finder_p(0)
	{}


    // destructor
    virtual ~sc_out_opt();


    // interface access shortcut methods

    // get the default event

    const sc_event& default_event() const
	{ return (*this)->default_event(); }


    // get the value changed event

    const sc_event& value_changed_event() const
	{ return (*this)->value_changed_event(); }


    // read the current value

    const data_type& read() const
	{ return (*this)->read(); }

    operator const data_type& () const
	{ return (*this)->read(); }


    // was there a value changed event?

    bool event() const
	{ return (*this)->event(); }


    // write the new value

    void write( const data_type& value_ )
	{ (*this)->write( value_ ); }

    this_type& operator = ( const data_type& value_ )
	{ (*this)->write( value_ ); return *this; }

    this_type& operator = ( const in_if_type& interface_ )
	{ (*this)->write( interface_.read() ); return *this; }

    this_type& operator = ( const in_port_type& port_ )
	{ (*this)->write( port_->read() ); return *this; }

    this_type& operator = ( const inout_port_type& port_ )
	{ (*this)->write( port_->read() ); return *this; }

    this_type& operator = ( const this_type& port_ )
	{ (*this)->write( port_->read() ); return *this; }


    // set initial value (can also be called when port is not bound yet)

    void initialize( const data_type& value_ );

    void initialize( const in_if_type& interface_ )
	{ initialize( interface_.read() ); }


    // called when elaboration is done
    /*  WHEN DEFINING THIS METHOD IN A DERIVED CLASS, */
    /*  MAKE SURE THAT THIS METHOD IS CALLED AS WELL. */

    virtual void end_of_elaboration();


    // (other) event finder method(s)

    sc_event_finder& value_changed() const
    {
        return sc_event_finder::cached_create
                ( m_change_finder_p, *this, &in_if_type::value_changed_event );
    }

    virtual const char* kind() const
        { return "sc_inout"; }

protected:

    data_type* m_init_val;

public:

    // called by sc_trace
    void add_trace_internal( sc_trace_file*, const std::string& ) const;

    void add_trace( sc_trace_file*, const std::string& ) const;

protected:

    mutable sc_trace_params_vec* m_traces;

private:
  mutable sc_event_finder* m_change_finder_p;

private:

    // disabled
    sc_out_opt( const this_type& );

};



template <class T>
inline
sc_out_opt<T>::~sc_out_opt()
{
    delete m_change_finder_p;
    delete m_init_val;
}

template <class T>
inline
void
sc_out_opt<T>::end_of_elaboration()
{
    if( m_init_val != 0 ) {
	write( *m_init_val );
	delete m_init_val;
	m_init_val = 0;
    }
    if( m_traces != 0 ) {
	for( int i = 0; i < (int)m_traces->size(); ++ i ) {
	    sc_trace_params* p = (*m_traces)[i];
	    in_if_type* iface = dynamic_cast<in_if_type*>( this->get_interface() );
	    sc_trace( p->tf, iface->read(), p->name );
	}
    }
}


#endif /* end of include guard: TANTRIKA_PORTS_H */

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



// sc_in_opt (modified from sc_in)
template <class T>
class sc_in_opt
: public sc_port<sc_signal_in_if<T>,1,SC_ZERO_OR_MORE_BOUND>
{
public:

    // typedefs

    typedef T                                             data_type;

    typedef sc_signal_in_if<data_type>                    if_type;
    typedef sc_port<if_type,1,SC_ZERO_OR_MORE_BOUND>       base_type;
    typedef sc_in_opt<data_type>                              this_type;
    typedef typename base_type::port_type                 base_port_type;

    typedef if_type                                       in_if_type;
    typedef base_type                                     in_port_type;
    typedef sc_signal_inout_if<data_type>                 inout_if_type;
    typedef sc_port<inout_if_type,1,SC_ZERO_OR_MORE_BOUND> inout_port_type;

public:

    // constructors

    sc_in_opt()
	: base_type(), m_traces( 0 ),
	  m_change_finder_p(0)
	{}

    explicit sc_in_opt( const char* name_ )
	: base_type( name_ ), m_traces( 0 ),
	  m_change_finder_p(0)
	{}

    explicit sc_in_opt( const in_if_type& interface_ )
        : base_type( const_cast<in_if_type&>( interface_ ) ), m_traces( 0 ),
	  m_change_finder_p(0)
        {}

    sc_in_opt( const char* name_, const in_if_type& interface_ )
	: base_type( name_, const_cast<in_if_type&>( interface_ ) ), m_traces( 0 ),
	  m_change_finder_p(0)
	{}

    explicit sc_in_opt( in_port_type& parent_ )
	: base_type( parent_ ), m_traces( 0 ),
	  m_change_finder_p(0)
	{}

    sc_in_opt( const char* name_, in_port_type& parent_ )
	: base_type( name_, parent_ ), m_traces( 0 ),
	  m_change_finder_p(0)
	{}

    explicit sc_in_opt( inout_port_type& parent_ )
	: base_type(), m_traces( 0 ),
	  m_change_finder_p(0)
	{ sc_port_base::bind( parent_ ); }

    sc_in_opt( const char* name_, inout_port_type& parent_ )
	: base_type( name_ ), m_traces( 0 ),
	  m_change_finder_p(0)
	{ sc_port_base::bind( parent_ ); }

    sc_in_opt( this_type& parent_ )
	: base_type( parent_ ), m_traces( 0 ),
	  m_change_finder_p(0)
	{}

    sc_in_opt( const char* name_, this_type& parent_ )
	: base_type( name_, parent_ ), m_traces( 0 ),
	  m_change_finder_p(0)
	{}


    // destructor

    virtual ~sc_in_opt()
	{
	    remove_traces();
	    delete m_change_finder_p;
	}


    // bind to in interface

    void bind( const in_if_type& interface_ )
	{ sc_port_base::bind( const_cast<in_if_type&>( interface_ ) ); }

    void bind( in_if_type& interface_ )
	{ this->bind( const_cast<const in_if_type&>( interface_ ) ); }

    void operator () ( const in_if_type& interface_ )
	{ this->bind( interface_ ); }


    // bind to parent in port

    void bind( in_port_type& parent_ )
        { sc_port_base::bind( parent_ ); }

    void operator () ( in_port_type& parent_ )
        { this->bind( parent_ ); }


    // bind to parent inout port

    void bind( inout_port_type& parent_ )
	{ sc_port_base::bind( parent_ ); }

    void operator () ( inout_port_type& parent_ )
	{ this->bind( parent_ ); }


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


    // (other) event finder method(s)

    sc_event_finder& value_changed() const
    {
        return sc_event_finder::cached_create
                ( m_change_finder_p, *this, &in_if_type::value_changed_event );
    }


    // called when elaboration is done
    /*  WHEN DEFINING THIS METHOD IN A DERIVED CLASS, */
    /*  MAKE SURE THAT THIS METHOD IS CALLED AS WELL. */

    virtual void end_of_elaboration();

    virtual const char* kind() const
        { return "sc_in"; }


    void add_trace( sc_trace_file*, const std::string& ) const;

    // called by sc_trace
    void add_trace_internal( sc_trace_file*, const std::string& ) const;

protected:

    void remove_traces() const;

    mutable sc_trace_params_vec* m_traces;

protected:

    // called by pbind (for internal use only)
    virtual int vbind( sc_interface& );
    virtual int vbind( sc_port_base& );

    // implement virtual base_type port-binding function
    //  - avoids warnings on some compilers
    //  - should only be called, when using sc_port_b explicitly
    //  - errors are detected during elaboration

    void bind( base_port_type& parent_ )
        { sc_port_base::bind( parent_ ); }


private:
  mutable sc_event_finder* m_change_finder_p;

private:

    // disabled
    sc_in_opt( const this_type& );
    this_type& operator = ( const this_type& );
};

template<typename T>
::std::ostream& operator << ( ::std::ostream& os, const sc_in_opt<T>& a )
{
    return os << a->read();
}

// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII


// called when elaboration is done

template <class T>
inline
void
sc_in_opt<T>::end_of_elaboration()
{
    if( m_traces != 0 ) {
	for( int i = 0; i < (int)m_traces->size(); ++ i ) {
	    sc_trace_params* p = (*m_traces)[i];
	    in_if_type* iface = dynamic_cast<in_if_type*>( this->get_interface() );
	    sc_trace( p->tf, iface->read(), p->name );
	}
	remove_traces();
    }
}


// called by sc_trace

template <class T>
inline
void
sc_in_opt<T>::add_trace_internal( sc_trace_file* tf_, const std::string& name_ )
const
{
    if( tf_ != 0 ) {
	if( m_traces == 0 ) {
	    m_traces = new sc_trace_params_vec;
	}
	m_traces->push_back( new sc_trace_params( tf_, name_ ) );
    }
}

template <class T>
inline
void
sc_in_opt<T>::add_trace( sc_trace_file* tf_, const std::string& name_ )
const
{
    sc_deprecated_add_trace();
    add_trace_internal(tf_, name_);
}

template <class T>
inline
void
sc_in_opt<T>::remove_traces() const
{
    if( m_traces != 0 ) {
	for( int i = (int)m_traces->size() - 1; i >= 0; -- i ) {
	    delete (*m_traces)[i];
	}
	delete m_traces;
	m_traces = 0;
    }
}


// called by pbind (for internal use only)

template <class T>
inline
int
sc_in_opt<T>::vbind( sc_interface& interface_ )
{
    return sc_port_b<if_type>::vbind( interface_ );
}

template <class T>
inline
int
sc_in_opt<T>::vbind( sc_port_base& parent_ )
{
    in_port_type* in_parent = dynamic_cast<in_port_type*>( &parent_ );
    if( in_parent != 0 ) {
	sc_port_base::bind( *in_parent );
	return 0;
    }
    inout_port_type* inout_parent = dynamic_cast<inout_port_type*>( &parent_ );
    if( inout_parent != 0 ) {
	sc_port_base::bind( *inout_parent );
	return 0;
    }
    // type mismatch
    return 2;
}


#endif /* end of include guard: TANTRIKA_PORTS_H */

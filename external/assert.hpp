// =====================================================================================
//
//       Filename:  assert.hpp
//
//    Description:  
//
//        Version:  1.0
//        Created:  Saturday 27 April 2019 04:09:12  IST
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Dilawar Singh (), dilawar.s.rajput@gmail.com
//   Organization:  NCBS Bangalore
//
// =====================================================================================

#ifndef ASSERT_H
#define ASSERT_H

#include <sstream>
#include <exception>
#include <stdexcept>
#include <limits>
#include <cmath>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <iomanip>
#include <ctime>
#include <algorithm>
#include <cstdlib>
#include <iostream>

using namespace std;

#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL 0
#endif

#define T_RESET       "\033[0m"
#define T_BLACK       "\033[30m"      /* Black */
#define T_RED         "\033[31m"      /* Red */
#define T_GREEN       "\033[32m"      /* Green */
#define T_YELLOW      "\033[33m"      /* Yellow */
#define T_BLUE        "\033[34m"      /* Blue */
#define T_MAGENTA     "\033[35m"      /* Magenta */
#define T_CYAN        "\033[36m"      /* Cyan */
#define T_WHITE       "\033[37m"      /* White */
#define T_BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define T_BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define T_BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define T_BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define T_BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define T_BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define T_BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define T_BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

using namespace std;


#undef SIMPLE_ASSERT
#if defined(SIMPLE_DISABLE_ASSERTS)
# define SIMPLE_ASSERT(expr) ((void)0)
#elif defined(SIMPLE_ENABLE_ASSERT_HANDLER)
namespace simpletest
{
    void assertion_failed(char const * expr,
            char const * function, char const * file, long line); // user defined
} // namespace simpletest

#define SIMPLE_ASSERT(expr) ((expr) \
        ? ((void)0) \
        : ::simpletest::assertion_failed(#expr, SIMPLE_CURRENT_FUNCTION, __FILE__, __LINE__))

#else
# include <assert.h> // .h to support old libraries w/o <cassert> - effect is the same
# define SIMPLE_ASSERT(expr) assert(expr)
#endif


#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
# define SIMPLE_CURRENT_FUNCTION __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
# define SIMPLE_CURRENT_FUNCTION __PRETTY_FUNCTION__
#elif defined(__FUNCSIG__)
# define SIMPLE_CURRENT_FUNCTION __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
# define SIMPLE_CURRENT_FUNCTION __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
# define SIMPLE_CURRENT_FUNCTION __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
# define SIMPLE_CURRENT_FUNCTION __func__
#else
# define SIMPLE_CURRENT_FUNCTION "(unknown)"
#endif

//--------------------------------------------------------------------------------------//
//                                   SIMPLE_ASSERT_MSG                                   //
//--------------------------------------------------------------------------------------//

# undef SIMPLE_ASSERT_MSG
#if defined(SIMPLE_DISABLE_ASSERTS) || defined(NDEBUG)

#define SIMPLE_ASSERT_MSG(expr, msg) ((void)0)

#elif defined(SIMPLE_ENABLE_ASSERT_HANDLER)

namespace simpletest
{
    void assertion_failed_msg(char const * expr, char const * msg,
            char const * function, char const * file, long line); // user defined
} // namespace simpletest

#define SIMPLE_ASSERT_MSG(expr, msg) ((expr) \
        ? ((void)0) \
        : ::simpletest::assertion_failed_msg(#expr, msg, SIMPLE_CURRENT_FUNCTION, __FILE__, __LINE__))

#else
#ifndef SIMPLE_ASSERT_HPP
#define SIMPLE_ASSERT_HPP

//  IDE's like Visual Studio perform better if output goes to std::cout or
//  some other stream, so allow user to configure output stream:
#ifndef SIMPLE_ASSERT_MSG_OSTREAM
# define SIMPLE_ASSERT_MSG_OSTREAM std::cerr
#endif

namespace simpletest
{ 
    namespace assertion 
    { 
        namespace detail
        {
            inline void assertion_failed_msg(char const * expr, char const * msg, char const * function,
                    char const * file, long line)
            {
                SIMPLE_ASSERT_MSG_OSTREAM
                    << "***** Internal Program Error - assertion (" << expr << ") failed in "
                    << function << ":\n"
                    << file << '(' << line << "): " << msg << std::endl;
#ifdef UNDER_CE
                // The Windows CE CRT library does not have abort() so use exit(-1) instead.
                std::exit(-1);
#else
                std::abort();
#endif
            }
        } // detail
    } // assertion
} // detail
#endif

#define SIMPLE_ASSERT_MSG(expr, msg) ((expr) \
        ? ((void)0) \
        : ::simpletest::assertion::detail::assertion_failed_msg(#expr, msg, \
            SIMPLE_CURRENT_FUNCTION, __FILE__, __LINE__))
#endif

//--------------------------------------------------------------------------------------//
//                                     SIMPLE_VERIFY                                     //
//--------------------------------------------------------------------------------------//

#undef SIMPLE_VERIFY

#if defined(SIMPLE_DISABLE_ASSERTS) || ( !defined(MOOSE_ENABLE_ASSERT_HANDLER) && defined(NDEBUG) )

# define SIMPLE_VERIFY(expr) ((void)(expr))

#else

# define SIMPLE_VERIFY(expr) MOOSE_ASSERT(expr)

/*-----------------------------------------------------------------------------
 *                                  SIMPLE_WARN
 *
 *  This macro accepts a stringstream like object as its second argument. First
 *  arguement is condition which must return a bool.
 *-----------------------------------------------------------------------------*/
#define SIMPLE_WARN(condition, msg) \
    if(! (condition) ) { \
        std::ostringstream ss; \
        ss << endl << "[WARN] " << msg; \
        std::cout << ss.str() \
        << std::endl; \
        std::cout << "\n\t|- " << "In function: " << SIMPLE_CURRENT_FUNCTION \
            << "\n\t+ In file: " << __FILE__ << ":" << __LINE__ << std::endl; \
    }

#endif

namespace simpletest {


    /**
     * @brief Enumerate type for debug and log.
     */
    enum serverity_level_ { 
        trace, debug, info , warning, fixme , error, fatal, failed 
    };

    static string levels_[9] = { 
        "TRACE", "DEBUG", "INFO", "WARNING", "FIXME" , "ERROR", "FATAL", "FAILED" 
    };

    /* 
     * ===  FUNCTION  ==============================================================
     *         Name:  mapToString
     *  Description:  GIven a map, return a string representation of it.
     *
     *  If the second argument is true then print the value with key. But default it
     *  is true.
     * ==============================================================================
     */

    template<typename A, typename B>
        string mapToString(const map<A, B>& m, bool value=true)
        {
            unsigned int width = 81;
            unsigned int mapSize = m.size();
            unsigned int size = 0;

            vector<string> row;

            /* Get the maximum size of any entry in map */
            stringstream ss;
            typename map<A, B>::const_iterator it;
            for(it = m.begin(); it != m.end(); it++)
            {
                ss.str("");
                ss << it->first;
                if(value)
                    ss << ": " << it->second;
                row.push_back(ss.str());
                if(ss.str().size() > size)
                    size = ss.str().size()+1;
            }

            unsigned int colums = width / size;
            ss.str("");

            size_t i = 0;
            for(unsigned int ii = 0; ii < row.size(); ii++)
            {
                if(i < colums)
                {
                    ss << setw(size+1) << row[ii];
                    i++;
                }
                else
                {
                    ss << endl;
                    i = 0;
                }
            }
            return ss.str();
        }

    inline string colored(string msg)
    {
        stringstream ss;
        ss << T_RED << msg << T_RESET;
        return ss.str();
    }

    inline string colored(string msg, string colorName)
    {
        stringstream ss;
        ss << colorName << msg << T_RESET;
        return ss.str();
    }

    inline string debugPrint(string msg, string prefix = "DEBUG"
            , string color=T_RESET, unsigned debugLevel = 0
            ) 
    {
        stringstream ss; ss.str("");
        if(debugLevel <= DEBUG_LEVEL)
        {
            ss << setw(debugLevel/2) << "[" << prefix << "] " 
                << color << msg << T_RESET;
        }
        return ss.str();
    }

    /*-----------------------------------------------------------------------------
     *  This function __dump__ a message onto console. Fills appropriate colors as
     *  needed. 
     *-----------------------------------------------------------------------------*/

    inline void __dump__(string msg, serverity_level_ type = debug, bool autoFormat = true)
    {
        stringstream ss;
        ss << setw(8) << levels_[type] << ": ";
        bool set = false;
        bool reset = true;
        string color = T_GREEN;
        if(type == warning || type == fixme )
            color = T_YELLOW;
        else if(type ==  debug )
            color = T_CYAN;
        else if(type == error || type == failed )
            color = T_RED;
        else if(type == info )
            color = T_MAGENTA;

        for(unsigned int i = 0; i < msg.size(); ++i)
        {
            if('`' == msg[i])
            {
                if(!set and reset) 
                {
                    set = true;
                    reset = false;
                    ss << color;
                }
                else if(set && !reset)
                {
                    reset = true;
                    set = false;
                    ss << T_RESET;
                }
            }
            else if('\n' == msg[i])
                ss << "\n | ";
            else
                ss << msg[i];
        }

        /*  Be safe than sorry */
        if(!reset)
            ss << T_RESET;
        cout << ss.str() << endl;
    }

    /*
     * Wrapper function around __dump__
     */
    inline void showInfo( string msg )
    {
        simpletest::__dump__( msg, simpletest::info );
    }

    inline void showWarn( string msg )
    {
        simpletest::__dump__(msg, simpletest::warning );
    }

    inline void showError( string msg )
    {
        simpletest::__dump__( msg, simpletest::error );
    }

    /**
     * @brief This macro only expands when not compiling for release.
     *
     * @param a Stream to write to logger /console.
     * @param t Type of the stream.
     * @return  Nothing.
     */

#ifdef  NDEBUG
#define LOG(a, t ) ((void)0);
#else      /* -----  not NDEBUG  ----- */
#define LOG(t, a) { \
    stringstream __ss__; \
    __ss__ << a; \
    simpletest::__dump__(__ss__.str(), t ); \
} 
#endif     /* -----  not NDEBUG  ----- */

    /*-----------------------------------------------------------------------------
     *  Log to a file, and also to console.
     *-----------------------------------------------------------------------------*/
    inline bool isBackTick(char a)
    {
        if('`' == a)
            return true;
        return false;
    }

    inline string formattedMsg(string& msg)
    {
        remove_if(msg.begin(), msg.end(), isBackTick);
        return msg;
    }

    /**
     * @brief Log to console (and to a log-file)
     *
     * @param msg String, message to be written.
     * @param type Type of the message.
     * @param redirectToConsole 
     * @param removeTicks
     */
    inline void log(string msg, serverity_level_ type = debug
            , bool redirectToConsole = true
            , bool removeTicks = true 
            )
    {

        if(redirectToConsole)
            __dump__(msg, type, true);

        /* remove any backtick from the string. */
        formattedMsg( msg );

        fstream logF;
        logF.open( "__moose__.log", ios::app);
        time_t rawtime; time(&rawtime);
        struct tm* timeinfo;
        timeinfo = localtime(&rawtime);

        logF << asctime(timeinfo) << ": " << msg;

        logF.close();
    }

}

inline bool doubleEq(double a, double b )
{
    return (std::fabs(a-b) < 1e-10);
            // * std::numeric_limits<double>::epsilon());
}

static ostringstream assertStream;

#define LOCATION(ss) \
    ss << "In function: " << SIMPLE_CURRENT_FUNCTION; \
    ss << " file: " << __FILE__ << ":" << __LINE__ << endl;  

#define EXPECT_TRUE( condition, msg) \
    if( !(condition) ) {\
        assertStream.str(""); \
        LOCATION( assertStream ); \
        assertStream << msg << endl; \
        simpletest::__dump__(assertStream.str(), simpletest::failed);\
    }

#define EXPECT_FALSE( condition, msg) \
    if( (condition) ) {\
        assertStream.str(""); \
        LOCATION( assertStream ); \
        assertStream << msg << endl; \
        simpletest::__dump__(assertStream.str(), simpletest::failed); \
    }

#define EXPECT_EQ(a, b, token)  \
    if( (a) != (b)) { \
        assertStream.str(""); \
        LOCATION(assertStream) \
        assertStream << "Expected " << b << ", received " << a ; \
        assertStream << token; \
        simpletest::__dump__(assertStream.str(), simpletest::failed); \
    }

#define EXPECT_NEQ(a, b, token)  \
    if( (a) == (b)) { \
        assertStream.str(""); \
        LOCATION(assertStream); \
        assertStream << "Not expected " << a << endl; \
        assertStream << token << endl; \
        simpletest::__dump__(assertStream.str(), simpletest::failed); \
    }

#define EXPECT_GT(a, b, token)  \
    if( (a) <= (b)) { \
        assertStream.str(""); \
        LOCATION(assertStream); \
        assertStream << "Expected greater than " << a << ", received " << b << endl; \
        assertStream << token << endl; \
        simpletest::__dump__(assertStream.str(), simpletest::failed); \
    }

#define EXPECT_GTE(a, b, token)  \
    if( (a) < (b)) { \
        assertStream.str(""); \
        LOCATION(assertStream); \
        assertStream << "Expected greater than or equal to " << a  \
            << ", received " << b << endl; \
        assertStream << token << endl; \
        simpletest::__dump__(assertStream.str(), simpletest::failed); \
    }

#define EXPECT_LT(a, b, token)  \
    if( (a) >= (b)) { \
        assertStream.str(""); \
        LOCATION(assertStream); \
        assertStream << "Expected less than " << a << ", received " << b << endl; \
        assertStream << token << endl; \
        simpletest::__dump__(assertStream.str(), simpletest::failed); \
    }

#define EXPECT_LTE(a, b, token)  \
    if( (a) < (b)) { \
        assertStream.str(""); \
        LOCATION(assertStream); \
        assertStream << "Expected less than or equal to " << a \
            << ", received " << b << endl; \
        assertStream << token << endl; \
        simpletest::__dump__(assertStream.str(), simpletest::failed); \
    }

#define ASSERT_TRUE( condition, msg) \
    if( !(condition) ) {\
        assertStream.str(""); \
        assertStream << msg << endl;  \
        throw std::runtime_error( assertStream.str() );\
    }

#define ASSERT_FALSE( condition, msg) \
    if( (condition) ) {\
        assertStream.str(""); \
        assertStream.precision( 9 ); \
        assertStream << msg << endl; \
        throw std::runtime_error(assertStream.str()); \
    }

#define ASSERT_LT( a, b, msg) \
    EXPECT_LT(a, b, msg); \
    assertStream.str(""); \
    assertStream.precision( 9 ); \
    assertStream << msg; \
    throw std::runtime_error( assertStream.str() ); \

#define ASSERT_EQ(a, b, token)  \
    if( ! doubleEq((a), (b)) ) { \
        assertStream.str(""); \
        LOCATION(assertStream) \
        assertStream <<"Expected "<<a<<", received "<<b<< ". Diff "<<a-b<<endl; \
        assertStream <<token<<endl; \
        throw std::runtime_error(assertStream.str()); \
    }

#define ASSERT_DOUBLE_EQ(token, a, b)  \
    if(! doubleEq(a, b) ) { \
        assertStream.str(""); \
        LOCATION(assertStream); \
        assertStream << "Expected " << b << ", received " << a  << endl; \
        assertStream << token; \
        simpletest::__dump__(assertStream.str(), simpletest::failed); \
        throw std::runtime_error( "float equality test simpletest::failed" ); \
    }

#define ASSERT_NEQ(a, b, token)  \
    if( (a) == (b)) { \
        assertStream.str(""); \
        LOCATION(assertStream); \
        assertStream << "Not expected " << a << endl; \
        assertStream << token << endl; \
        throw std::runtime_error(assertStream.str()); \
}



#endif /* end of include guard: ASSERT_H */

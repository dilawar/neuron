// =====================================================================================
//
//       Filename:  tantrika.h
//
//    Description:  Tantrika header file.
//
//        Version:  1.0
//        Created:  01/05/19 08:55:48 PM IST
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Dilawar Singh (), dilawar.s.rajput@gmail.com
//   Organization:  NCBS Bangalore
//
// =====================================================================================

#ifndef TANTRIKA_H
#define TANTRIKA_H

#include <boost/preprocessor/stringize.hpp>

#include "IAF.h"
#include "SynapseGroup.h"
#include "Network.h"

// This is from cmake.
#ifdef LOG_LEVEL
#define SPDLOG_ACTIVE_LEVEL SPD_LOG_ ## LOG_LEVEL
#endif

namespace tantrika {

    void create_network();
}

#endif /* end of include guard: TANTRIKA_H */

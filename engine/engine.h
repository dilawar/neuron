/*
 *
 *    Description:  Numerical functions.
 *
 *         Author:  Dilawar Singh (), dilawars@ncbs.res.in
 *   Organization:  NCBS Bangalore
 *
 */

#ifndef ENGINE_H
#define ENGINE_H

#include <cmath>


namespace tantrika {

    inline double alpha(const double dt, const double tau)
    {
        return (dt/tau)*exp(-dt/tau);
    }

}

#endif /* end of include guard: ENGINE_H */


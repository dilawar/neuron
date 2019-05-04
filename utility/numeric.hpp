/***
 *    Description:  Numerical utilities.
 *
 *        Created:  2019-04-27

 *         Author:  Dilawar Singh <dilawars@ncbs.res.in>
 *   Organization:  NCBS Bangalore
 *        License:  MIT License
 */

#ifndef NUMERICS_H
#define NUMERICS_H

#include <numeric>
#include <functional>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>

using namespace boost::accumulators;

template<typename T=double>
std::valarray<T> min_max_mean_std(const std::vector<T>& data)
{
    accumulator_set<T, features<tag::min, tag::max, tag::mean, tag::variance> > acc;

    std::for_each(data.begin(), data.end(), std::bind<void>(std::ref(acc), std::placeholders::_1));

    return {boost::accumulators::min(acc), boost::accumulators::max(acc)
            , boost::accumulators::mean(acc), std::pow(boost::accumulators::variance(acc), 0.5) };
}


#endif /* end of include guard: NUMERICS_H */

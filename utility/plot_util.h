/*
 *       Filename:  plot_util.h
 *
 *         Author:  Dilawar Singh (), dilawars@ncbs.res.in
 *   Organization:  NCBS Bangalore
 */

#ifndef PLOT_UTIL_H
#define PLOT_UTIL_H

#include "../external/gnuplot/gnuplot-iostream.h"

#include <vector>
#include <map>

void plot_vectors(const std::vector<double>& t, const std::vector<double>& y);

void map2csv( const std::map<std::string, std::vector<double>>& data
        , const std::string& csvname
        , const std::string& delim = ","
        );

#endif /* end of include guard: PLOT_UTIL_H */

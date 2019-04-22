/*
 *    Description:  Plot utility.
 *
 *         Author:  Dilawar Singh (), dilawars@ncbs.res.in
 *   Organization:  NCBS Bangalore
 */

#include "plot_util.h"

#include <boost/tuple/tuple.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/range/combine.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/stream.hpp>
#include <iostream>

using namespace std;
namespace io = boost::iostreams;

void plot_vectors(const vector<double>& t, const vector<double>& y)
{
    std::cout << "Plotting vector of size: " << y.size() << std::endl;
    Gnuplot gp;
    gp.send1d(boost::make_tuple(t, y));
}

void map2csv( const map<string, vector<double>>& data, const string& csvname, const string& delim)
{
    io::stream_buffer<io::file_sink> buf(csvname);
    std::ostream csvF(&buf);

    vector<string> header;
    size_t nVals = 10000000;

    // Write header.
    for(auto v : data)
    {
        header.push_back(v.first);
        nVals = min(v.second.size(), nVals);
    }

    csvF << boost::algorithm::join(header, delim) << delim << Endl;

    for (size_t i = 0; i < nVals; i++) 
    {
        for(auto v : data)
            csvF << v.second[i] << delim;

        csvF << endl;
    }
    buf.close();
}

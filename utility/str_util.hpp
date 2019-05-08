/***
 *    Description:  String utilities.
 *
 *        Created:  2019-05-08

 *         Author:  Dilawar Singh <dilawars@ncbs.res.in>
 *   Organization:  NCBS Bangalore
 *        License:  MIT License
 */

#ifndef STR_UTIL_H
#define STR_UTIL_H

#include <string>

using namespace std;

/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  Remove [\d+] at the end of path.
 *
 * @Param path
 *
 * @Returns   
 */
/* ----------------------------------------------------------------------------*/
inline string groupName(const string& path)
{
    assert( ! path.empty() );

    string res(path.c_str());
    if( res.back() == ']')
        res = res.substr(0, res.rfind('['));

    assert(res.size() > 0);

    return res;
}



#endif /* end of include guard: STR_UTIL_H */

/***
 *    Description:  Utility function.
 *
 *        Created:  2019-05-10

 *         Author:  Dilawar Singh <dilawars@ncbs.res.in>
 *   Organization:  NCBS Bangalore
 *        License:  MIT License
 */

#include "sc_utils.hpp"
#include <systemc>
#include <regex>

string printHeir(sc_core::sc_object const* obj, size_t level)
{
    string s(level, ' ');
    s += obj->name();

    auto children = obj->get_child_objects();
    // s += "    " + to_string(children.size());
    if(children.size() == 0)
        return s;

    for(const sc_core::sc_object* t : children)
    {
        s += "\n";
        s += printHeir(t, level+1);
    }

    return s;
}

// Remove special char which systemc does not like.
string sanitizePath(const string& path)
{
    std::regex badChar("[.]");
    return std::regex_replace(path, badChar, "+");
}


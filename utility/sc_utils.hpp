/***
 *    Description:  SystemC helper functions.
 *
 *        Created:  2019-05-09

 *         Author:  Dilawar Singh <dilawars@ncbs.res.in>
 *   Organization:  NCBS Bangalore
 *        License:  MIT License
 */

#ifndef SC_UTILS_H
#define SC_UTILS_H

#include <systemc>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

using namespace std;

// Find port when name is given.

    template<typename T=sc_core::sc_port_base>
T* findPort(sc_core::sc_object const* obj, const string& name, const string& kind="") 
{
    for (sc_core::sc_object* v : obj->get_child_objects())
    {
        if(string(v->basename()) == name)
        {
            if(kind.empty())
                return dynamic_cast<T*>(v);
            else
            {
                if(string(v->kind()) == kind)
                    return dynamic_cast<T*>(v);
            }
        }
    }
    return nullptr;
}

template<typename T=sc_core::sc_port_base>
vector<T*> getPorts(sc_core::sc_object const* obj, const string& kind="") 
{
    std::vector<T*> res;
    for (sc_core::sc_object* v : obj->get_child_objects()) {
        if(kind.empty() || string(v->kind())==kind) {
            res.push_back(dynamic_cast<T*>(v));
        }
    }
    return res;
}

template<typename T=sc_core::sc_port_base>
void findPorts(sc_core::sc_object const* obj, const string& kind, vector<string>& names) 
{
    for (const sc_core::sc_object* v : obj->get_child_objects())
        if(string(v->kind())==kind)
            names.push_back((boost::format("%1%(%2%)")%v->basename()%kind).str());
}



    template<typename T=sc_core::sc_port_base>
string availablePortsCSV(sc_core::sc_object const* obj, const string& kind="") 
{
    vector<string> sports;
    if(! kind.empty()) {
        findPorts(obj, kind, sports);
    }
    else {
        findPorts(obj, "sc_in", sports);
        findPorts(obj, "sc_inout", sports);
        findPorts(obj, "sc_out", sports);
    }
    return boost::algorithm::join(sports, ", ");
}

// Print hierarchy.
std::string printHeir(sc_core::sc_object const* obj, size_t level=0);

// Remove characters from the path which systemc does not like.
string sanitizePath(const string& path);

#endif /* end of include guard: SC_UTILS_H */


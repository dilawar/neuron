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

using namespace std;

// Find port when name is given.

    template<typename T=sc_port_base>
T* findPort(sc_object* obj, const string& name, const string& kind) 
{
    for (sc_object* v : obj->get_child_objects())
    {
        std::string pKind = v->kind(); 
        if(string(v->basename()) == name && pKind==kind)
        {
            if(kind == "sc_in")
                return dynamic_cast<T*>(v);
            else if(kind == "sc_out")
                return dynamic_cast<T*>(v);
            else if(kind == "sc_inout")
                return dynamic_cast<T*>(v);
            else
                return dynamic_cast<T*>(v);
        }
    }
    return nullptr;
}

    template<typename T=sc_port_base>
vector<T*> findPorts(sc_object const* obj, const string& kind) 
{
    std::vector<T*> res;
    for (sc_object* v : obj->get_child_objects())
        if(string(v->kind())==kind)
            res.push_back(dynamic_cast<T*>(v));
    return res;
}

    template<typename T=sc_port_base>
string availablePortsCSV(sc_object const* obj, const string& kind) 
{
    vector<string> sports;
    for (sc_object* v : obj->get_child_objects())
        if(string(v->kind())==kind)
            sports.push_back(v->basename());
    return boost::algorithm::join(sports, ", ");
}


#endif /* end of include guard: SC_UTILS_H */


from libcpp.string cimport string

cdef extern from "../include/Synapse.h":
    cdef cppclass Synapse:
        Synapse()
        Synapse(string, double, double, double, string)
        string path()


from libcpp.string cimport string

cdef extern from "../include/Network.h":
    cdef cppclass Network:
        Network() except +
        Network(const char*) except +
        string path()

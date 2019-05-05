# distutil: language=c++
# language_level=3

from libcpp.string cimport string
from libcpp.vector cimport vector

cimport Synapse 


cdef extern from "../include/Network.h":

    cdef cppclass Network:
        Network() except +
        Network(const char*) except +
        string path()
        void addSynapseAlpha(string path)
        void addSynapseAlpha(SynapseAlpha* ptr)
        void.addSynapseExp(string path)
        void.addSynapseExp(SynapseExp* path)
        void getSynapses()

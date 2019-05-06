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
        void addSynapse(string path)
        void getSynapses()

        void SynapseGroup(int N, double gbar, double tau, double Esyn, string stype)  
        int start(double) 

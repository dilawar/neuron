from libcpp.string cimport string

cdef extern from "../include/Synapse.h":
    cdef cppclass SynapseBase:
        SynapseBase()
        string path()

    cdef cppclass SynapseAlpha(SynapseBase):
        SynapseAlpha() except +
        SynapseAlpha(string name, double x0, double y0, double tau)

    cdef cppclass SynapseExp(SynapseBase):
        SynapseExp() except +
        SynapseExp(string name, double x0, double y0, double tau)


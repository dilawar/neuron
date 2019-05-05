# distutils: language = c++
# language_level = 3

#  from synapse cimport SynapseExp
from SynapseExp cimport SynapseExp

# Create a Cython extension type which holds a C++ instance
# as an attribute and create a bunch of forwarding methods
# Python extension type.
cdef class Synapse:
    cdef SynapseExp* c_syn_exp 

    def __cinit__(self, str name, double x0, double y0, double tau):
        self.c_syn_exp = new SynapseExp(name, x0, y0, tau)

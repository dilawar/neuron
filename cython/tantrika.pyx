# distutils: language = c++
# language_level = 3
from cython.operator import dereference as deref

from libcpp.string cimport string
from libcpp.vector cimport vector
from libcpp.memory cimport unique_ptr, shared_ptr

#  Import but with different name so that we can have the same Python name. For
#  more see https://cython.readthedocs.io/en/latest/src/userguide/external_C_code.html#resolve-conflicts
from Network cimport Network as CppNetwork
from Synapse cimport Synapse as CppSynapse

cdef class Synapse:
    cdef unique_ptr[CppSynapse] syn

    def __init__(self, name, gbar=1e-9, tau=1e-3, Esyn=0.0, stype='alpha'):
        self.syn.reset(
                new CppSynapse(name.encode('utf8')
                    , gbar, tau, Esyn
                    , stype.decode('utf8'))
                )

# Now here is our wrapper class.
cdef class Network:
    cdef unique_ptr[CppNetwork] thisptr 
    synapses = []

    def __cinit__(self, name):
        self.thisptr.reset(new CppNetwork(name.encode('utf8')))

    def __repr__(self):
        return str(type(self)) + " Path:" + self.path() 

    def path(self):
        return self.c_network.path().decode('utf8')

    def getSynapses(self):
        return self.synapses

    def addSynapse(self, path, stype='alpha'):
        syn = Synapse(path, stype=stype)
        self.synapses.append(syn)

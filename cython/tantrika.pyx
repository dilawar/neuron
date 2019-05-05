# distutils: language = c++
# language_level = 3
from cython.operator import dereference as deref

from libcpp.string cimport string
from libcpp.vector cimport vector
from libcpp.memory cimport unique_ptr

#  Import but with different name so that we can have the same Python name. For
#  more see https://cython.readthedocs.io/en/latest/src/userguide/external_C_code.html#resolve-conflicts
from Network cimport Network as CppNetwork
from Synapse cimport SynapseBase, SynapseAlpha, SynapseExp
from Synapse cimport SynapseExp

cdef class Synapse:
    cdef unique_ptr[SynapseExp] synExp;
    cdef unique_ptr[SynapseAlpha] synAlpha;

    def __init__(self, name, gbar=1e-9, tau=1e-3, Esyn=0.0, stype='alpha'):
        if stype == 'alpha':
            self.synAlpha.reset(new SynapseAlpha(name, gbar, tau, Esyn))
        else:
            self.synExp.reset(new SynapseExp(name, gbar, tau, Esyn))


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
        deref(self.thisptr).getSynapses()

    cdef addSynapseAlpha(self, SynapseAlpha* ptr):
        deref(self.thisptr).addSynapseAlpha(ptr)

    cdef addSynapseExp(self, SynapseExp* ptr)
        deref(self.thisptr).addSynapseExp(ptr)

    def addSynapse(self, path, ctype='alpha'):
        syn = Synapse(path, ctype)
        if ctype == 'alpha':
            self.addSynapseAlpha(path.encode('utf8'))
        else:
            self.addSynapseExp(path.encode('utf8'))



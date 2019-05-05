# distutils: language = c++
# language_level = 3
from cython.operator import dereference as dref

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

    def __cinit__(self, const char* name, double x0, double y0, double tau,
            stype='alpha'):
        if stype == 'alpha':
            self.synAlpha.reset(new SynapseAlpha(name, x0, y0, tau))
        else:
            self.synExp.reset(new SynapseExp(name, x0, y0, tau))

# Now here is our wrapper class.
cdef class Network:
    cdef unique_ptr[CppNetwork] thisptr 

    def __cinit__(self, name):
        self.thisptr.reset(new CppNetwork(name.encode('utf8')))
        self.synapses = []

    def __repr__(self):
        return str(type(self)) + " Path:" + self.path() 

    def path(self):
        return self.c_network.path().decode('utf8')

    def getSynapses(self):
        return self.synapses

    cdef addSynapseAlpha(self, string path):
        dref(self.thisptr).addSynapseAlpha(path)
        #  cdef SynapseAlpha* syn = self.c_network.addSynapseAlpha(path)
        #  self.synapses.append(SynapseAlpha(syn))

    cdef addSynapseExp(self, string path):
        dref(self.thisptr).addSynapseExp(path)
        #  cdef SynapseExp* syn = self.c_network.addSynapseExp(path)
        #  self.synapses.append(syn)

    def addSynapse(self, path, ctype='alpha'):
        if ctype == 'alpha':
            self.addSynapseAlpha(path.encode('utf8'))
        else:
            self.addSynapseExp(path.encode('utf8'))



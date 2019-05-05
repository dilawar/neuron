# distutils: language = c++
# language_level = 3

from libcpp.string cimport string
from libcpp.vector cimport vector

#  Import but with different name so that we can have the same Python name. For
#  more see https://cython.readthedocs.io/en/latest/src/userguide/external_C_code.html#resolve-conflicts
from Network cimport Network as CppNetwork

# Now here is our wrapper class.
cdef class Network:
    cdef CppNetwork* c_network 

    def __cinit__(self, name):
        self.c_network = new CppNetwork(name.encode('utf8'))

    def __dealloc__(self):
        del self.c_network

    def __repr__(self):
        return str(type(self)) + " Path:" + self.path() 

    def path(self):
        return self.c_network.path().decode('utf8')


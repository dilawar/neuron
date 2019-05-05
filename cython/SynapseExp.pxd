cdef extern from "../include/SynapseExp.h":

    cdef cppclass SynapseExp:
        SynapseExp(const char*, double, double, double) except +


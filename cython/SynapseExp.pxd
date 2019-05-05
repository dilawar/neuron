cdef extern from "../include/SynapseExp.h":

    cdef cppclass SynapseExp:
        SynapseExp(string, double, double, double) except +


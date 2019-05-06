file(GLOB SRCS *.cpp)
add_library(neuron OBJECT ${SRCS})
add_dependencies(neuron libsystemc)

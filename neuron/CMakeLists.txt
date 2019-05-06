file(GLOB SRCS *.cpp)
add_library(neuron STATIC ${SRCS})
add_dependencies(neuron libsystemc)

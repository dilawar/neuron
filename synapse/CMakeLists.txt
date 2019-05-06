file(GLOB SRCS *.cpp)
add_library(synapse OBJECT ${SRCS})
add_dependencies(synapse libsystemc)

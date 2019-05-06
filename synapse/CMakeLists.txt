file(GLOB SRCS *.cpp)
add_library(synapse STATIC ${SRCS})
add_dependencies(synapse libsystemc)

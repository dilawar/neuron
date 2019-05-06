file(GLOB SRCS *.cpp)
add_library(network OBJECT ${SRCS})
add_dependencies(network libsystemc)

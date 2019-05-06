file(GLOB SRCS *.cpp)
add_library(network STATIC ${SRCS})
add_dependencies(network libsystemc)

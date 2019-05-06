file(GLOB SRCS *.cpp)
add_library(utility OBJECT ${SRCS})
add_dependencies(utility libsystemc)

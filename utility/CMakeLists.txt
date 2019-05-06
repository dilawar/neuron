file(GLOB SRCS *.cpp)
add_library(utility STATIC ${SRCS})
add_dependencies(utility libsystemc)

file(GLOB SRCS *.cpp)
add_library(engine ${SRCS})
add_dependencies(engine libsystemc)

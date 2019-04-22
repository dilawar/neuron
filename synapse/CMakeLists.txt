cmake_minimum_required(VERSION 3.0)
file(GLOB SRCS *.cpp)
add_library(synapse STATIC  ${SRCS})


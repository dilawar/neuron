cmake_minimum_required(VERSION 3.0)
project(tantrika)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(SystemC REQUIRED)
find_package(Boost COMPONENTS log)

include_directories( ${SystemC_INCLUDE_DIR} )

# Add main executable.
add_executable(tantrika exp/experiment1.cpp)

target_link_libraries(tantrika ${SystemC_LIBRARIES} ${Boost_LIBRARIES})


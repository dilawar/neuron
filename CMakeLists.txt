cmake_minimum_required(VERSION 3.0)
project(tantrika)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
add_definitions(-std=c++14 -Wall)

# SystemC
find_package(SystemC REQUIRED)
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L${SystemC_LIBRARY_DIRS}")

find_package(Boost COMPONENTS log)
set(LIBS_TO_LINKS  ${Boost_LIBRARIES} ${SystemC_LIBRARIES})

# Configuration macros.
add_definitions( -DBOOST_LOG_DYN_LINK )

include_directories( ${SystemC_INCLUDE_DIR} )

# subdirectories.
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/synapse)



# Add main executable.
add_executable(tantrika exp/experiment1.cpp)

target_link_libraries(tantrika ${LIBS_TO_LINKS})


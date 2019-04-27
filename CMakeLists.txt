cmake_minimum_required(VERSION 3.0)
project(tantrika)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


# SystemC as external project.
include(ExternalProject)
set(LIBSYSTEMC_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/external)
ExternalProject_Add(libsystemc
    PREFIX ${LIBSYSTEMC_ROOT}
    INSTALL_DIR ${LIBSYSTEMC_ROOT}/install
    URL ${LIBSYSTEMC_ROOT}/systemc-2.3.3.tar.gz
    CONFIGURE_COMMAND ${CMAKE_COMMAND} 
        -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
        -DBUILD_SHARED_LIBS=OFF 
        -DCMAKE_CXX_STANDARD=14
        <SOURCE_DIR>
    BUILD_COMMAND $(MAKE) -j4
    )
set(SystemC_INCLUDE_DIR ${LIBSYSTEMC_ROOT}/install/include)
set(SystemC_LIBRARY ${LIBSYSTEMC_ROOT}/install/lib64/libsystemc.a)
add_definitions(-Wall -std=c++14)

find_package(Boost COMPONENTS log system filesystem iostreams)
# Configuration macros.
add_definitions( -DBOOST_LOG_DYN_LINK)

include_directories( ${SystemC_INCLUDE_DIR} )

# subdirectories.
add_subdirectory(synapse)
add_subdirectory(neuron)
add_subdirectory(utility)
add_subdirectory(engine)

set(LIBS_TO_LINKS utility 
    synapse
    ${Boost_LIBRARIES} 
    ${SystemC_LIBRARY}
    )


# Add test directory at the end
enable_testing()
add_subdirectory(tests)

# Add main executable.
add_executable(tantrika exp/experiment1.cpp)
target_link_libraries(tantrika ${LIBS_TO_LINKS})

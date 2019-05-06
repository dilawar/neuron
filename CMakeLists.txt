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
        -DCMAKE_CXX_FLAGS=-fPIC
        -DBUILD_SHARED_LIBS=OFF 
        -DCMAKE_CXX_STANDARD=14
        -DDISABLE_COPYRIGHT_MESSAGE=ON
        <SOURCE_DIR>
    BUILD_COMMAND $(MAKE)
    )
set(SystemC_INCLUDE_DIR ${LIBSYSTEMC_ROOT}/install/include)
set(SystemC_LIBRARY ${LIBSYSTEMC_ROOT}/install/lib64/libsystemc.a)
add_definitions(-Wall -std=c++14)

find_package(Boost COMPONENTS log system filesystem iostreams)
# Configuration macros.
add_definitions( -DBOOST_LOG_DYN_LINK)

include_directories( ${SystemC_INCLUDE_DIR} )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include )

add_definitions(-fPIC)

# subdirectories.
add_subdirectory(synapse)
add_subdirectory(neuron)
add_subdirectory(utility)
add_subdirectory(engine)
# add_subdirectory(network)

set(STATIC_LIBRARIES 
    synapse
    neuron
    utility 
    # network
    ${SystemC_LIBRARY}
    )
set(SYSTEM_SHARED_LIBS ${Boost_LIBRARIES} )
message(STATUS " Libraries to link ${STATIC_LIBRARIES}")

# Build cython extention.
if(GENERATE_PYTHON_BINDING)
    add_subdirectory(cython)
    add_dependencies(pytantrika tantrika)
endif(GENERATE_PYTHON_BINDING)

# Add main executable.
add_library(tantrika STATIC main.cpp)
add_dependencies(tantrika libsystemc)
if(APPLE)
    target_link_libraries(tantrika
        "-Wl,-all_load"
        ${MOOSE_LIBRARIES}
        ${STATIC_LIBRARIES}
        )
else(APPLE)
    target_link_libraries(tantrika
        "-Wl,--whole-archive"
        ${STATIC_LIBRARIES}
        "-Wl,--no-whole-archive"
        )
endif(APPLE)
target_link_libraries(tantrika ${SYSTEM_SHARED_LIBS} ${CMAKE_DL_LIBS})

# Add test directory at the end
enable_testing()
add_subdirectory(tests)

# install
include(GNUInstallDirs)
install(TARGETS tantrika 
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

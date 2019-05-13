cmake_minimum_required(VERSION 3.0)
project(tantrika)

option(GENERATE_PYTHON_BINDING "Generate Cython bindings" ON)
option(ADDRESS_SANITIZE "Use address sanitizer" OFF)


set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_CXX_STANDARD 14)

add_definitions(-Wall -std=c++${CMAKE_CXX_STANDARD})

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
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_CXX_STANDARD=14
        -DDISABLE_COPYRIGHT_MESSAGE=ON
        <SOURCE_DIR>
    BUILD_COMMAND $(MAKE)
    )
set(SystemC_INCLUDE_DIR ${LIBSYSTEMC_ROOT}/install/include)
set(SystemC_LIBRARY ${LIBSYSTEMC_ROOT}/install/lib64/libsystemc.a)

include_directories( ${SystemC_INCLUDE_DIR} )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/external)

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    if(ADDRESS_SANITIZE)
        add_definitions(-fsanitize=address)
        add_link_options(-fsanitize=address)
    endif()
    add_definitions(-DLOG_LEVEL=debug)
endif()

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# subdirectories.
add_subdirectory(synapse)
add_subdirectory(neuron)
add_subdirectory(utility)
add_subdirectory(stimulus)
add_subdirectory(engine)
add_subdirectory(network)

set(CMAKE_SHARED_LIBS ${Boost_LIBRARIES} )

# Build cython extention.
if(GENERATE_PYTHON_BINDING)
    add_subdirectory(cython)
    add_dependencies(cytantrika tantrika libsystemc)
endif(GENERATE_PYTHON_BINDING)

# TANTRIKA library. Somehow using a SHARED lib and creating a whole-archive with
# system C with libsystemc.a double free segv.
add_library(tantrika STATIC main.cpp
    $<TARGET_OBJECTS:synapse>
    $<TARGET_OBJECTS:neuron>
    $<TARGET_OBJECTS:stimulus>
    $<TARGET_OBJECTS:utility>
    $<TARGET_OBJECTS:network>
    )

add_dependencies(tantrika libsystemc)
target_link_libraries(tantrika ${SystemC_LIBRARY})
target_link_libraries(tantrika ${CMAKE_SHARED_LIBS})

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

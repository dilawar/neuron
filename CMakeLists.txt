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
    URL ${LIBSYSTEMC_ROOT}/systemc-2.3.4.tar.gz
    CONFIGURE_COMMAND ${CMAKE_COMMAND} 
        -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
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

find_package(Boost COMPONENTS log system filesystem iostreams REQUIRED)
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

set(LIBS_TO_LINKS 
    synapse
    neuron
    utility 
    ${Boost_LIBRARIES} 
    ${SystemC_LIBRARY}
    )

# Add main executable.
add_library(tantrika_static STATIC base/tantrika.cpp)
add_library(tantrika SHARED base/tantrika.cpp)
add_dependencies(tantrika libsystemc)

set_target_properties(tantrika_static PROPERTIES LIBRARY_OUTPUT_NAME tantrika)
target_link_libraries(tantrika_static ${LIBS_TO_LINKS})
target_link_libraries(tantrika ${LIBS_TO_LINKS})


# Add test directory at the end
enable_testing()
add_subdirectory(tests)

# install
include(GNUInstallDirs)
install(TARGETS tantrika_static tantrika
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(Tantrika)

SET(CMAKE_VERBOSE_MAKEFILE OFF)

# Create directories to install systemc
SET(SYSTEMC_INSTALL_DIR ${CMAKE_SOURCE_DIR}/systemc)
FILE(MAKE_DIRECTORY ${SYSTEMC_INSTALL_DIR})

LINK_DIRECTORIES(
    ${SYSTEMC_INSTALL_DIR}/lib
    ${SYSTEMC_INSTALL_DIR}/lib-linux
    ${SYSTEMC_INSTALL_DIR}/lib-linux64
    )

ADD_DEFINITIONS("-Wall -DDEBUG --std=c++11")
ADD_DEFINITIONS("-DMONITOR")

OPTION(PROFILING "Profile the code" OFF)
IF(PROFILING)
    MESSAGE("++ gprof is enabled")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")
ENDIF()

INCLUDE(ExternalProject)
ExternalProject_Add(
   systemc-2.3
   PREFIX ${CMAKE_SOURCE_DIR}/build_systemc-2.3
   SOURCE_DIR  ${CMAKE_SOURCE_DIR}/systemc-2.3
   CONFIGURE_COMMAND ${CMAKE_SOURCE_DIR}/systemc-2.3/configure --prefix=${SYSTEMC_INSTALL_DIR}
   BUILD_COMMAND make
   INSTALL_COMMAND make install
) 

INCLUDE_DIRECTORIES(${SYSTEMC_INSTALL_DIR}/include
    ${CMAKE_SOURCE_DIR}/includes
    ${CMAKE_SOURCE_DIR}/synapse
    ${CMAKE_SOURCE_DIR}/types
    )

OPTION(ENABLE_TEST "Enable tests" ON)
IF(ENABLE_TEST)
    ADD_DEFINITIONS("-DENABLE_TEST")
ENDIF()

#ADD_SUBDIRECTORY(models)
ADD_SUBDIRECTORY(synapse)
ADD_SUBDIRECTORY(log)

ADD_EXECUTABLE(Tantrika main.cc)
ADD_DEPENDENCIES(Tantrika systemc-2.3)

# Link sub-libraries.
TARGET_LINK_LIBRARIES(Tantrika
    synapse
    SimpleTest
    )


# Target Tantrika depends on external project systemc-2.3. Build and install it
# first before continue building Tantrika.
TARGET_LINK_LIBRARIES(Tantrika systemc m)




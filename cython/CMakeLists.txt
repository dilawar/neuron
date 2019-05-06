cmake_minimum_required(VERSION 3.0)
project(pytantrika)

find_package(PythonInterp REQUIRED)
find_package(PythonLibs REQUIRED)
include_directories(${Python_INCLUDE_DIRS})
include_directories(${SystemC_INCLUDE_DIR})

set(CMAKE_TANTRIKAPY_INCLUDE_DIR 
    "'${SystemC_INCLUDE_DIR}', '${PYTHON_INCLUDE_PATH}'"
    )
set(CMAKE_TANTRIKAPY_LIBDIR ${CMAKE_BINARY_DIR})
message(STATUS "Library directory: ${CMAKE_TANTRIKAPY_LIBDIR}")

# set module extensiton. default is .so. 
execute_process( COMMAND
    ${Python_EXECUTABLE} -c
"import importlib.machinery
print(importlib.machinery.EXTENSION_SUFFIXES[0])"
    OUTPUT_VARIABLE PYTHON_SO_EXTENSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
if(NOT PYTHON_SO_EXTENSION)
    message(STATUS "Python so ext could not be determined. Using default .so")
    set(PYTHON_SO_EXTENSION ".so")
endif()
message(STATUS "Python so extension ${PYTHON_SO_EXTENSION}" )

set(TANTRIKA_MODULE tantrika${PYTHON_SO_EXTENSION})

# Copy releavant files to build directory.
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in
    ${CMAKE_CURRENT_SOURCE_DIR}/setup.py)
add_custom_command(OUTPUT ${TANTRIKA_MODULE}
    COMMAND ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/setup.py 
        build_ext 
        --build-lib ${CMAKE_BINARY_DIR}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating ${TANTRIKA_MODULE}"
    )

add_custom_target(pytantrika ALL DEPENDS ${TANTRIKA_MODULE})
add_dependencies(pytantrika tantrika)
add_dependencies(pytantrika libsystemc)

enable_testing()
file(GLOB _pytest_files "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_*.py")
foreach(_pytest_file ${_pytest_files})
    get_filename_component(_fname ${_pytest_file} NAME)
    set(_test_name "cython_${_fname}")
    message(STATUS "Adding test ${_test_name}")
    add_test(NAME "${_test_name}"
        COMMAND ${Python_EXECUTABLE} ${_pytest_file}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        )
    set_tests_properties(${_test_name} 
        PROPERTIES ENVIRONMENT 
            "PYTHONPATH=${CMAKE_BINARY_DIR};LD_LIBRARY_PATH=${CMAKE_BINARY_DIR}"
        )
endforeach()

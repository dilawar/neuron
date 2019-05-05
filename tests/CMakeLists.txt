cmake_minimum_required(VERSION 3.0)
find_package(Boost COMPONENTS unit_test_framework)

foreach(_test test_hebbian test_synapses test_iaf)
    add_executable(${_test} ${_test}.cpp)
    target_compile_definitions(${_test} PRIVATE "BOOST_TEST_DYN_LINK=1")
    target_compile_definitions(${_test} PRIVATE "ENABLE_TEST=ON")
    target_link_libraries(${_test} tantrika ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
    add_test(NAME run_${_test} 
        COMMAND $<TARGET_FILE:${_test}> 
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        )
endforeach()

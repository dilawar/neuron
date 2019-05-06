cmake_minimum_required(VERSION 3.0)

foreach(_test test_hebbian test_synapses test_iaf)
    add_executable(${_test} ${_test}.cpp)
    target_compile_definitions(${_test} PRIVATE "BOOST_TEST_DYN_LINK=1")
<<<<<<< HEAD
    target_compile_definitions(${_test} PRIVATE "ENABLE_TEST=ON")
    target_link_libraries(${_test} tantrika
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
=======
    target_compile_definitions(${_test} PRIVATE "ENABLE_TEST=ON -g -O0")
    target_link_libraries(${_test} tantrika)
>>>>>>> asn_error
    add_test(NAME run_${_test} 
        COMMAND $<TARGET_FILE:${_test}> 
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        )
endforeach()

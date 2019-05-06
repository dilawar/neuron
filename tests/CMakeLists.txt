cmake_minimum_required(VERSION 3.0)

foreach(_test test_hebbian test_synapses test_iaf)
    add_executable(${_test} ${_test}.cpp)
    add_dependencies(${_test} tantrika)
    target_compile_definitions(${_test} PRIVATE "ENABLE_TEST=ON")
    target_link_libraries(${_test} tantrika)
    add_test(NAME run_${_test} 
        COMMAND $<TARGET_FILE:${_test}> 
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        )
endforeach()

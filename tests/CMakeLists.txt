foreach(_test test_hebbian test_synapses)
    add_executable(${_test}  ${_test}.cpp)
    target_link_libraries(${_test} ${LIBS_TO_LINKS})
    add_test(NAME run_${_test} 
        COMMAND $<TARGET_FILE:${_test}> 
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        )
endforeach()

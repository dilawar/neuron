cmake_minimum_required(VERSION 3.0)

file(GLOB TEST_SRCS test_*.cpp)
foreach(_test_src ${TEST_SRCS})
    get_filename_component(_test ${_test_src} NAME_WE)
    add_executable(${_test} ${_test_src})
    target_compile_definitions(${_test} PRIVATE "ENABLE_TEST=ON")
    target_link_libraries(${_test} tantrika)
    add_test(NAME run_${_test}
        COMMAND $<TARGET_FILE:${_test}>
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        )
endforeach()

# foreach(_test test_network test_synapses test_iaf)
    # add_executable(${_test} ${_test}.cpp)
    # add_dependencies(${_test} tantrika)
    # target_compile_definitions(${_test} PRIVATE "ENABLE_TEST=ON")
    # target_link_libraries(${_test} tantrika)
    # add_test(NAME run_${_test}
        # COMMAND $<TARGET_FILE:${_test}>
        # WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        # )
# endforeach()

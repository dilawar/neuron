foreach(_test test_hebbian test_exp_syn)
    add_executable(${_test}  ${_test}.cpp)
    target_link_libraries(${_test} ${LIBS_TO_LINKS})
endforeach()

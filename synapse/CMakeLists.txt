cmake_minimum_required(VERSION 3.0)
# add_library(synapse test_simple_synapse.cc)

add_executable(test_synapse  test_simple_synapse.cc)
target_link_libraries(test_synapse ${LIBS_TO_LINKS})

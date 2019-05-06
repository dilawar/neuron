add_library(neurons integrate_and_fire.h integrate_and_fire.cc)
add_dependencies(neurons libsystemc)

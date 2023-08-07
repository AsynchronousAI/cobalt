cmake_minimum_required(VERSION 3.0...3.5)

project(minicobalt)

find_library(LIBM_LIBRARIES NAMES m)

add_executable(minicobalt ../../minicobalt.c)
if (LIBM_LIBRARIES)
  target_link_libraries(minicobalt ${LIBM_LIBRARIES})
endif()

if (MSVC)
  set_target_properties(minicobalt PROPERTIES RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_CURRENT_BINARY_DIR}>)
endif()
cmake_minimum_required(VERSION 3.16)

# Define a "project", providing a description and a programming language
project(raytracer
    VERSION 1.0
    DESCRIPTION "Ray Tracing Project"
    LANGUAGES CXX
)

enable_testing()

# Our "project" will be able to build an executable out of a C++ source file
add_executable(raytracer
    src/raytracer.cpp
)

add_executable(colorTest
  test/colors.cpp
)

# This is needed if we keep .h files in the "include" directory
target_include_directories(trace PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

add_library(trace
    src/colors.cpp
)

target_link_libraries(
    raytracer PUBLIC trace 
)

# This program runs the automatic tests on the "Color" class
add_executable(colorTest
  test/colors.cpp
  )
# The test program too needs to be linked to "trace"
target_link_libraries(colorTest PUBLIC trace)

add_test(NAME colorTest 
    COMMAND colorTest
)

# Force the compiler to use the C++17 standard
target_compile_features(raytracer PUBLIC cxx_std_17)
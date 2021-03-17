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

# This program runs the automatic tests on the "Color" class
add_executable(colorTest
  test/colors.cpp
)

add_executable(HdrImageTest
  test/HdrImage.cpp
)

# Our project will be able to build a library
add_library(trace
    src/colors.cpp
    src/HdrImage.cpp
)

# This is needed if we keep .h files in the "include" directory
target_include_directories(trace PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

# Target commands tells which dependencies targer exec have
target_link_libraries(
    raytracer PUBLIC trace 
)

# The test program too needs to be linked to "trace"
target_link_libraries(colorTest PUBLIC trace)
target_link_libraries(HdrImageTest PUBLIC trace)

add_test(NAME colorTest 
    COMMAND colorTest
)
add_test(NAME HdrImageTest
    COMMAND HdrImageTest
)

# Force the compiler to use the C++17 standard
target_compile_features(raytracer PUBLIC cxx_std_17)
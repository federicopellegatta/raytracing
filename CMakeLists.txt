cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENTIONS OFF)

# Define a "project", providing a description and a programming language
project(raytracer
    VERSION 1.0
    DESCRIPTION "Ray Tracing Project"
    LANGUAGES CXX
)

# set PKG_CONFIG_PATH to include lib directory in my conda env
# in order to be found by pkg-config
set(ENV{PKG_CONFIG_PATH} "$ENV{CONDA_PREFIX}/lib/pkgconfig/:$ENV{PKG_CONFIG_PATH}")

# Add libgd library (copying from the links on Tomasi's slides)
# the `pkg_check_modules` function is created with this call
find_package(PkgConfig REQUIRED)
# these calls create special `PkgConfig::<MODULE>` variables
pkg_check_modules(PKG_GDLIB REQUIRED IMPORTED_TARGET gdlib>=2.2.5)

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

add_executable(geometryTest
  test/geometry.cpp
)

add_executable(rayTest
  test/ray.cpp
)

add_executable(cameraTest
  test/camera.cpp
)

add_executable(imagetracerTest
  test/imagetracer.cpp
)

add_executable(shapesTest
  test/shapes.cpp
)

add_executable(materialsTest
  test/materials.cpp
)

add_executable(renderTest
  test/render.cpp
)

add_executable(pcgTest
  test/pcg.cpp
)

# Our project will be able to build a library
add_library(trace
    src/colors.cpp
    src/HdrImage.cpp
    src/format.cc
    src/geometry.cpp
    src/shapes.cpp
    src/world.cpp
    src/materials.cpp
    src/render.cpp
    src/pcg.cpp
)

# This is needed if we keep .h files in the "include" directory
target_include_directories(trace PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

# This is needed for our trace library because it use libgd to 
# convert Hdr Images to Ldr images. 
# No need to specify include_dirs or compile options, cmake takes
# care of it all
target_link_libraries(trace PUBLIC PkgConfig::PKG_GDLIB)

# Target commands tells which dependencies targer exec have
target_link_libraries(
    raytracer PUBLIC 
    trace
)

# The test program too needs to be linked to "trace"
target_link_libraries(colorTest PUBLIC trace)
target_link_libraries(HdrImageTest PUBLIC trace)
target_link_libraries(geometryTest PUBLIC trace)
target_link_libraries(rayTest PUBLIC trace)
target_link_libraries(cameraTest PUBLIC trace)
target_link_libraries(imagetracerTest PUBLIC trace)
target_link_libraries(shapesTest PUBLIC trace)
target_link_libraries(materialsTest PUBLIC trace)
target_link_libraries(renderTest PUBLIC trace)
target_link_libraries(pcgTest PUBLIC trace)





add_test(NAME colorTest 
    COMMAND colorTest
)
add_test(NAME HdrImageTest
    COMMAND HdrImageTest
)
add_test(NAME geometryTest
    COMMAND geometryTest
)
add_test(NAME rayTest
    COMMAND rayTest
) 
add_test(NAME cameraTest
    COMMAND cameraTest
)
add_test(NAME imagetracerTest
    COMMAND imagetracerTest
)
add_test(NAME shapesTest
    COMMAND shapesTest 
)
add_test(NAME materialsTest
    COMMAND materialsTest 
)
add_test(NAME renderTest
    COMMAND renderTest 
)
add_test(NAME pcgTest
    COMMAND pcgTest 
)

# Force the compiler to use the C++17 standard
target_compile_features(raytracer PUBLIC cxx_std_17)
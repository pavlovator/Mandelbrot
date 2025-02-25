# Minimum version of CMake
cmake_minimum_required(VERSION 3.10)

# Project name
project(MandelbrotSet)

# Specify the C++ standard to C++17
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Specify the source files
set(SOURCES src/main.cpp)

# Add the Raylib package (You might need to set the path if Raylib is not installed globally)
find_package(raylib REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include)

# Add the executable
add_executable(mandelbrot ${SOURCES})

# Link the Raylib library (and other dependencies)
target_link_libraries(mandelbrot raylib GL m pthread dl rt X11)

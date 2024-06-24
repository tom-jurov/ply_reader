# MIT License

# Copyright (c) 2024 Tomas Jurov

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
cmake_minimum_required(VERSION 3.16...3.26)
project(ply_reader VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Find Python and pybind11
find_package(Python COMPONENTS Interpreter Development.Module REQUIRED)
find_package(pybind11 CONFIG REQUIRED)

# Add FetchContent module for Eigen3 and miniply
include(FetchContent)

# Fetch Eigen3
FetchContent_Declare(
    eigen
    GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
    GIT_TAG 3.4.0
)
FetchContent_MakeAvailable(eigen)

# Fetch miniply
FetchContent_Declare(
    miniply
    GIT_REPOSITORY https://github.com/vilya/miniply.git
    GIT_TAG master # Adjust to a specific release or commit hash if needed
)
FetchContent_MakeAvailable(miniply)

# Define the C++ source files
set(SOURCES
    ${CMAKE_SOURCE_DIR}/src/ply_reader.cpp
    ${CMAKE_SOURCE_DIR}/src/ply_reader_pybind.cpp
    ${miniply_SOURCE_DIR}/miniply.cpp
)

# Create the Python extension module
pybind11_add_module(ply_reader MODULE ${SOURCES})

# Link against pybind11 and miniply
target_link_libraries(ply_reader PRIVATE pybind11::module)

# Include directories
target_include_directories(ply_reader PRIVATE 
    ${Python3_INCLUDE_DIRS} 
    ${CMAKE_SOURCE_DIR}/include 
    ${eigen_SOURCE_DIR} 
    ${miniply_SOURCE_DIR}  # Assuming miniply has an 'include' directory
)

# Specify output directories
set_target_properties(ply_reader PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build
)

# Optionally, install the Python module
install(TARGETS ply_reader DESTINATION .)

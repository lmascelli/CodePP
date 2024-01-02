cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(CodePP LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
include("${CMAKE_SOURCE_DIR}/cmake/compiler.cmake")

option(COMPONENT_MEAS "include the MEAs implementatiions" ON)
option(COMPONENT_HDF5 "include the hdf5 interface" ON)
option(COMPONENT_HDF5_INSPECTOR "include the hdf5 inspector utility library" ON)
option(COMPONENT_CSV "include the csv interface" OFF)

add_subdirectory("src/codepp")

set(CodePP_SOURCES "src/main.cpp")
add_executable(CodePP ${CodePP_SOURCES})
target_link_libraries(CodePP PRIVATE CodePPLib)

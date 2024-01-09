cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(CodePP
        VERSION 0.1.0
        LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
include("${CMAKE_SOURCE_DIR}/cmake/compiler.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/utils.cmake")

option(COMPONENT_MEAS "include the MEAs implementatiions" ON)
option(COMPONENT_HDF5 "include the hdf5 interface" ON)
option(COMPONENT_HDF5_INSPECTOR "include the hdf5 inspector utility library" ON)
option(COMPONENT_CSV "include the csv interface" OFF)
option(BUILD_PYCODE "build the PyCode Python wrapper" ON)

add_subdirectory("src/codepp")

set(CodePP_SOURCES "src/main.cpp")
add_executable(CodePP ${CodePP_SOURCES})
target_link_libraries(CodePP PRIVATE CodePPLib)

if(BUILD_PYCODE AND CMAKE_BUILD_TYPE STREQUAL "Release")
  add_subdirectory("pycode")
endif()

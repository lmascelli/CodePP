# Create a library interface that can be configured with the requested options
add_library(CodePPLib INTERFACE)

# FORMAT LIBRARY
include(FetchContent)
set(FMT_TAG 10.1.1)
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG ${FMT_TAG})
FetchContent_MakeAvailable(fmt)

# ##############################################################################
# CodePPLib_Core library

set(CodePPLib_Core_HEADER_LIST "types/signal.hpp" "types/mea.hpp")
list(TRANSFORM CodePPLib_Core_HEADER_LIST
     PREPEND "${CodePP_SOURCE_DIR}/include/codepp/core/")

set(CodePPLib_Core_SOURCES "types/signal.cpp")
list(TRANSFORM CodePPLib_Core_SOURCES
     PREPEND "${CodePP_SOURCE_DIR}/src/codepp/core/")

add_library(CodePPLib_Core ${CodePPLib_Core_SOURCES})
target_link_libraries(CodePPLib_Core INTERFACE all_warnings warnings_are_errors
                                               fmt::fmt)
target_include_directories(CodePPLib_Core
                           INTERFACE "${CMAKE_SOURCE_DIR}/include")

# ##############################################################################
# CodePPLib_Meas library

set(CodePPLib_Meas_HEADER_LIST "meas/mea60.hpp")
list(TRANSFORM CodePPLib_Meas_HEADER_LIST
     PREPEND "${CodePP_SOURCE_DIR}/include/codepp/")

set(CodePPLib_Meas_SOURCES "meas/mea60.cpp")
list(TRANSFORM CodePPLib_Meas_SOURCES
     PREPEND "${CodePP_SOURCE_DIR}/src/codepp/")

add_library(CodePPLib_Meas ${CodePPLib_Meas_SOURCES})
target_link_libraries(CodePPLib_Meas PUBLIC CodePPLib_Core)

# ##############################################################################
# codepplib_hdf5 library
find_package(HDF5 CONFIG REQUIRED)

set(CodePPLib_hdf5_HEADER_LIST "hdf5/h5utils.hpp" "hdf5/h5analog.hpp"
                               "hdf5/h5content.hpp")
list(TRANSFORM CodePPLib_hdf5_HEADER_LIST
     PREPEND "${CodePP_SOURCE_DIR}/include/codepp/")

set(CodePPLib_hdf5_SOURCES "hdf5/h5utils.cpp" "hdf5/h5analog.cpp"
                           "hdf5/h5content.cpp")
list(TRANSFORM CodePPLib_hdf5_SOURCES
     PREPEND "${CodePP_SOURCE_DIR}/src/codepp/")

add_library(CodePPLib_hdf5 ${CodePPLib_hdf5_SOURCES})
target_link_libraries(CodePPLib_hdf5 PUBLIC CodePPLib_Core hdf5::hdf5)

# codepplib_hdf5_inspector library
set(CodePPLib_hdf5_inspector_HEADER_LIST "hdf5/type_inspector.hpp")
list(TRANSFORM CodePPLib_hdf5_inspector_HEADER_LIST
     PREPEND "${CodePP_SOURCE_DIR}/include/codepp/")

set(CodePPLib_hdf5_inspector_SOURCES "hdf5/type_inspector.cpp")
list(TRANSFORM CodePPLib_hdf5_inspector_SOURCES
     PREPEND "${CodePP_SOURCE_DIR}/src/codepp/")

add_library(CodePPLib_hdf5_inspector ${CodePPLib_hdf5_inspector_SOURCES})
target_link_libraries(CodePPLib_hdf5_inspector PRIVATE CodePPLib_hdf5)


# ##############################################################################
# CodePPLib_csv library

set(CodePPLib_csv_HEADER_LIST "csv/csv.hpp")
list(TRANSFORM CodePPLib_csv_HEADER_LIST
     PREPEND "${CodePP_SOURCE_DIR}/include/codepp/")

set(CodePPLib_csv_SOURCES "csv/csv.cpp")
list(TRANSFORM CodePPLib_csv_SOURCES
     PREPEND "${CodePP_SOURCE_DIR}/src/codepp/")

add_library(CodePPLib_csv ${CodePPLib_csv_SOURCES})
target_link_libraries(CodePPLib_csv PUBLIC CodePPLib_Core)


# ##############################################################################
# Link selected libraries to the main CodePPLib

target_link_libraries(CodePPLib INTERFACE CodePPLib_Core)
if(COMPONENT_MEAS)
  target_link_libraries(CodePPLib INTERFACE CodePPLib_Meas)
endif()
if(COMPONENT_HDF5)
  target_link_libraries(CodePPLib INTERFACE CodePPLib_hdf5)
  if(COMPONENT_HDF5_INSPECTOR)
    target_link_libraries(CodePPLib INTERFACE CodePPLib_hdf5)
  endif()
endif()
if(COMPONENT_CSV)
    target_link_libraries(CodePPLib INTERFACE CodePPLib_csv)
endif()

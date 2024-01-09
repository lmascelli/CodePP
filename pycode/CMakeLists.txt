# find_package(Python3 COMPONENTS REQUIRED Development NumPy)
find_package(pybind11 CONFIG REQUIRED)

if (COMPONENT_MEAS)
  set(PyCode_with_meas 1)
else()
  set(PyCode_with_meas 0)
endif()
if (COMPONENT_HDF5)
  set(PyCode_with_hdf5 1)
else()
  set(PyCode_with_hdf5 0)
endif()
if (COMPONENT_CSV)
  set(PyCode_with_csv 1)
else()
  set(PyCode_with_csv 0)
endif()

configure_file(pycodeConfig.hpp.in pycodeConfig.hpp)

set(PyCode_HEADER_LIST "")
list(TRANSFORM PyCode_HEADER_LIST
    PREPEND "${PROJECT_SOURCE_DIR}/include/PyCode/")

set(PyCode_SOURCES "pycode.cpp")
list(TRANSFORM PyCode_SOURCES PREPEND "${PROJECT_SOURCE_DIR}/pycode/")

pybind11_add_module(PyCode ${PyCode_SOURCES})
target_include_directories(PyCode PRIVATE 
                                  "${PROJECT_SOURCE_DIR}/include"
                                  "${PyCode_BINARY_DIR}"
                                  "${pybind11_INCLUDE_DIRS}"
)
target_link_libraries(PyCode PRIVATE CodePPLib)

install(PyCode ${PROJECT_SOURCE_DIR])

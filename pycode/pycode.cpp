#include <pybind11/pybind11.h>

namespace py = pybind11;

auto add(int i, int j) -> int {
  return i + j;
}

PYBIND11_MODULE(pycode, m) {
  m.doc() = "pycode with pybind11 example tutorial";

  m.def("add", &add, "A function that adds two numbers");
}

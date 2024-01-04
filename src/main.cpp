#include <codepp/hdf5/h5export.hpp>
#include <prelude.hpp>

auto main() -> int {
  string urca = "urcaaaa";
  auto file = CodePP::HDF5::H5File("test.h5");
  auto group = file.add_group("nants ingoniama");
  group.add_string("var1", "urcaaa");
  group.add_ints("va2", {0, 1, 2});
  auto sub_group = group.add_struct("struct 1");
  sub_group.add_floats("var 3", {1, 2});
  return 0;
}

#pragma once
#include <codepp/hdf5/h5utils.hpp>

namespace CodePP::HDF5 {

template <typename T> class H5Field {
public:
  friend class H5Struct;

private:
  H5Field(hid_t group, const string &name, const T &data)
      : name(std::move(name)) {
    (void)group; // will be used when creating the dataset
    fmt::println("H5Field: unimplemented datatype {}", typeid(data).name());
    exit(1);
  }

  H5Field(hid_t group, const string &name, const vector<T> &data)
      : name(std::move(name)) {
    (void)group; // will be used when creating the dataset
    fmt::println("H5Field: unimplemented datatype {}", typeid(data).name());
    exit(1);
  }

  string name;
  Type type;
  hid_t datatype;
  hid_t dataspace;
  hid_t dataset;
};

class H5Struct {
public:
  friend class H5File;
  void add_string(const string &name, const string &value);
  void add_ints(const string &name, const vector<long long> &value);
  void add_floats(const string &name, const vector<float> &value);
  auto add_struct(const string &name) -> H5Struct&;

private:
  H5Struct(hid_t parent, const string &name);
  vector<H5Field<string>> strings;
  vector<H5Field<long long>> int_arrays;
  vector<H5Field<float>> float_arrays;
  vector<H5Struct> groups;

  hid_t group;
};

class H5File {
public:
  H5File(const string &filename);
  auto add_group(const string &name) -> H5Struct &;

private:
  vector<H5Struct> groups;
  hid_t file;
};
} // namespace CodePP::HDF5

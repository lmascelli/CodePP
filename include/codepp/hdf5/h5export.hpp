#pragma once
#include <codepp/hdf5/h5utils.hpp>
#include <codepp/core/types/signal.hpp>

namespace CodePP::HDF5 {

////////////////////////////////////////////////////////////////////////////////
///                            GENERAL TYPE STORAGE
////////////////////////////////////////////////////////////////////////////////

template <typename T> class H5Field {
public:
  H5Field(const H5Field &copied) = delete;
  auto operator=(const H5Field &copied) = delete;

  H5Field(H5Field &&moved) {
    moved.moved = true;

    name = std::move(moved.name);
    type = moved.type;
    datatype = moved.datatype;
    dataspace = moved.dataspace;
    dataset = moved.dataset;
  }

  auto operator=(H5Field &&moved) -> H5Field & {
    moved.moved = true;

    name = std::move(moved.name);
    type = moved.type;
    datatype = moved.datatype;
    dataspace = moved.dataspace;
    dataset = moved.dataset;

    return moved;
  }

  ~H5Field();

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

  H5Field(hid_t group, const string &name, const std::span<const T> &data)
      : name(std::move(name)) {
    (void)group; // will be used when creating the dataset
    fmt::println("H5Field: unimplemented datatype {}", typeid(data).name());
    exit(1);
  }
  bool moved = false;

  string name;
  Type type;
  hid_t datatype;
  hid_t dataspace;
  hid_t dataset;
};

class H5Struct {
public:
  H5Struct(const H5Struct &copied) = delete;
  auto operator=(const H5Struct &copied) = delete;
  H5Struct(H5Struct &&moved);
  auto operator=(H5Struct &&moved) -> H5Struct &;
  ~H5Struct();

  void add_string(const string &name, const string &value);
  void add_ints(const string &name, const vector<long long> &value);
  void add_uints(const string &name, const vector<unsigned long long> &value);
  void add_floats(const string &name, const vector<float> &value);
  void add_floats(const string &name, const std::span<const float> &value);
  auto add_struct(const string &name) -> H5Struct &;

  friend class H5File;

private:
  bool moved = false;

  H5Struct(hid_t parent, const string &name);
  vector<H5Field<string>> strings;
  vector<H5Field<long long>> int_arrays;
  vector<H5Field<unsigned long long>> uint_arrays;
  vector<H5Field<float>> float_arrays;
  vector<H5Struct> groups;

  hid_t group;
};

class H5File {
public:
  H5File(const string &filename);
  H5File(const H5File &copied) = delete;
  auto operator=(const H5File &copied) = delete;
  H5File(H5File &&moved);
  auto operator=(H5File &&moved) -> H5File &;

  auto add_struct(const string &name) -> H5Struct &;
  ~H5File();

private:
  bool moved = false;
  vector<H5Struct> groups;
  hid_t file;
};

////////////////////////////////////////////////////////////////////////////////
///                            CODEPP TYPE STORAGE
////////////////////////////////////////////////////////////////////////////////

void H5Signal(H5Struct& group, const Signal& signal);
} // namespace CodePP::HDF5

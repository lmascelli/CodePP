#include <codepp/hdf5/h5export.hpp>
#include <hdf5.h>

namespace CodePP::HDF5 {

////////////////////////////////////////////////////////////////////////////////
///                               H5FIELD
////////////////////////////////////////////////////////////////////////////////

template <typename T> H5Field<T>::~H5Field() {
  if (not moved) {
    H5Dclose(dataset);
    H5Sclose(dataspace);
    H5Tclose(datatype);
  }
}

template <>
H5Field<string>::H5Field(hid_t group, const string &name, const string &data)
    : name(std::move(name)) {
  type = Type::STRING;
  auto strlen = data.size();

  // create datatype
  datatype = H5Tcopy(H5T_C_S1);
  H5Tset_size(datatype, strlen);
  // create dataspace
  array<hsize_t, 1> dsize = {1};
  dataspace = H5Screate_simple(1, dsize.data(), nullptr);
  // create dataset
  dataset = H5Dcreate(group, name.c_str(), datatype, dataspace, H5P_DEFAULT,
                      H5P_DEFAULT, H5P_DEFAULT);

  // write data in the dataset

  H5Dwrite(dataset, datatype, H5S_ALL, dataspace, H5P_DEFAULT, data.data());
}

template <>
H5Field<long long>::H5Field(hid_t group, const string &name,
                            const vector<long long> &data)
    : name(std::move(name)) {
  type = Type::INTEGER;
  auto veclen = data.size();

  // create datatype
  datatype = H5Tcopy(H5T_NATIVE_LLONG);
  H5Tset_size(datatype, 8);
  // create dataspace
  array<hsize_t, 1> dsize = {veclen};
  dataspace = H5Screate_simple(1, dsize.data(), nullptr);
  // create dataset
  dataset = H5Dcreate(group, name.c_str(), datatype, dataspace, H5P_DEFAULT,
                      H5P_DEFAULT, H5P_DEFAULT);

  // write data in the dataset
  H5Dwrite(dataset, datatype, H5S_ALL, dataspace, H5P_DEFAULT, data.data());
}

template <>
H5Field<unsigned long long>::H5Field(hid_t group, const string &name,
                                     const vector<unsigned long long> &data)
    : name(std::move(name)) {
  type = Type::INTEGER;
  auto veclen = data.size();

  // create datatype
  datatype = H5Tcopy(H5T_NATIVE_ULLONG);
  H5Tset_size(datatype, 8);
  // create dataspace
  array<hsize_t, 1> dsize = {veclen};
  dataspace = H5Screate_simple(1, dsize.data(), nullptr);
  // create dataset
  dataset = H5Dcreate(group, name.c_str(), datatype, dataspace, H5P_DEFAULT,
                      H5P_DEFAULT, H5P_DEFAULT);

  // write data in the dataset
  H5Dwrite(dataset, datatype, H5S_ALL, dataspace, H5P_DEFAULT, data.data());
}

template <>
H5Field<float>::H5Field(hid_t group, const string &name,
                        const vector<float> &data)
    : name(std::move(name)) {
  type = Type::INTEGER;
  auto veclen = data.size();

  // create datatype
  datatype = H5Tcopy(H5T_NATIVE_FLOAT);
  H5Tset_size(datatype, 4);
  // create dataspace
  array<hsize_t, 1> dsize = {veclen};
  dataspace = H5Screate_simple(1, dsize.data(), nullptr);
  // create dataset
  dataset = H5Dcreate(group, name.c_str(), datatype, dataspace, H5P_DEFAULT,
                      H5P_DEFAULT, H5P_DEFAULT);

  // write data in the dataset

  H5Dwrite(dataset, datatype, H5S_ALL, dataspace, H5P_DEFAULT, data.data());
}

template <>
H5Field<float>::H5Field(hid_t group, const string &name,
                        const std::span<const float> &data)
    : name(std::move(name)) {
  type = Type::INTEGER;
  auto veclen = data.size();

  // create datatype
  datatype = H5Tcopy(H5T_NATIVE_FLOAT);
  H5Tset_size(datatype, 4);
  // create dataspace
  array<hsize_t, 1> dsize = {veclen};
  dataspace = H5Screate_simple(1, dsize.data(), nullptr);
  // create dataset
  dataset = H5Dcreate(group, name.c_str(), datatype, dataspace, H5P_DEFAULT,
                      H5P_DEFAULT, H5P_DEFAULT);

  // write data in the dataset

  H5Dwrite(dataset, datatype, H5S_ALL, dataspace, H5P_DEFAULT, data.data());
}

////////////////////////////////////////////////////////////////////////////////
///                               H5STRUCT
////////////////////////////////////////////////////////////////////////////////

H5Struct::H5Struct(H5Struct &&moved) {
  moved.moved = true;
  group = moved.group;

  strings = std::move(moved.strings);
  int_arrays = std::move(moved.int_arrays);
  uint_arrays = std::move(moved.uint_arrays);
  float_arrays = std::move(moved.float_arrays);
  groups = std::move(moved.groups);
}

auto H5Struct::operator=(H5Struct &&moved) -> H5Struct & {
  moved.moved = true;
  group = moved.group;

  strings = std::move(moved.strings);
  int_arrays = std::move(moved.int_arrays);
  uint_arrays = std::move(moved.uint_arrays);
  float_arrays = std::move(moved.float_arrays);
  groups = std::move(moved.groups);

  return moved;
}

H5Struct::~H5Struct() {
  if (not moved)
    H5Gclose(group);
}

H5Struct::H5Struct(hid_t parent, const string &name) {
  group =
      H5Gcreate(parent, name.c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
}

void H5Struct::add_string(const string &name, const string &value) {
  strings.emplace_back(H5Field{group, name, value});
}

void H5Struct::add_ints(const string &name, const vector<long long> &value) {
  int_arrays.emplace_back(H5Field{group, name, value});
}

void H5Struct::add_uints(const string &name,
                         const vector<unsigned long long> &value) {
  uint_arrays.emplace_back(H5Field{group, name, value});
}

void H5Struct::add_floats(const string &name, const vector<float> &value) {
  float_arrays.emplace_back(H5Field{group, name, value});
}

void H5Struct::add_floats(const string &name,
                          const std::span<const float> &value) {
  float_arrays.emplace_back(H5Field{group, name, value});
}

auto H5Struct::add_struct(const string &name) -> H5Struct & {
  groups.emplace_back(H5Struct{group, name});
  return groups.at(groups.size() - 1);
}

////////////////////////////////////////////////////////////////////////////////
///                               H5FILE
////////////////////////////////////////////////////////////////////////////////

H5File::H5File(const string &filename) {
  file = H5Fcreate(filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
  if (file <= 0) {
    fmt::println("ERROR H5File: failed opening {}", filename);
    exit(1);
  }
}

H5File::H5File(H5File &&moved) {
  groups = std::move(moved.groups);
  file = moved.file;
  moved.moved = true;
}

auto H5File::operator=(H5File &&moved) -> H5File & {
  moved.moved = true;
  groups = std::move(moved.groups);
  file = moved.file;
  return moved;
}

H5File::~H5File() {
  if (not moved)
    H5Fclose(file);
}

auto H5File::add_struct(const string &name) -> H5Struct & {
  groups.emplace_back(H5Struct{file, name});
  return groups.at(groups.size() - 1);
}

////////////////////////////////////////////////////////////////////////////////
///                            CODEPP TYPE STORAGE
////////////////////////////////////////////////////////////////////////////////

/// H5Signal
void H5Signal([[maybe_unused]] H5Struct &str,
              [[maybe_unused]] const Signal &signal) {
  str.add_floats("data", signal.get_data());
  str.add_floats("sampling frequency", {signal.sampling_frequency});
}
} // namespace CodePP::HDF5

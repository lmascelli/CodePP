#include <H5Fpublic.h>
#include <H5Ppublic.h>
#include <H5Tpublic.h>
#include <codepp/hdf5/h5export.hpp>
#include <hdf5.h>

namespace CodePP::HDF5 {

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

void H5Struct::add_floats(const string &name, const vector<float> &value) {
  float_arrays.emplace_back(H5Field{group, name, value});
}

auto H5Struct::add_struct(const string &name) -> H5Struct & {
  groups.emplace_back(H5Struct{group, name});
  return groups.at(groups.size() - 1);
}

H5File::H5File(const string &filename) {
  file = H5Fcreate(filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
  if (file <= 0) {
    fmt::println("ERROR H5File: failed opening {}", filename);
    exit(1);
  }
}

auto H5File::add_group(const string &name) -> H5Struct & {
  groups.emplace_back(H5Struct{file, name});
  return groups.at(groups.size() - 1);
}

} // namespace CodePP::HDF5

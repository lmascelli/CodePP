#pragma once
#include <codepp/core/types/signal.hpp>
#include <codepp/hdf5/h5utils.hpp>
#include <prelude.hpp>

namespace CodePP::HDF5 {

/// info_channel_struct
/// holds informations contained in the InfoChannel Dataset of an Analog Stream
///
/// With this information it is possible to convert the raw values of the ADC
/// stored in the DataChannel dataset and obtain an array of float values
/// representing the actual voltage recorded.
struct info_channel_struct {
  int channel_id;
  int row_index;
  int group_id;
  int electrode_group;
  char *label;
  char *raw_data_type;
  char *unit;
  int exponent;
  int ad_zero;
  long long int tick;
  long long int conversion_factor;
  int adc_bits;
  char *high_pass_filter_type;
  char *high_pass_filter_cutoff;
  int high_pass_filter_order;
  char *low_pass_filter_type;
  char *low_pass_filter_cutoff;
  int low_pass_filter_order;

  /// print()
  /// created for debugging pourpose. prints the value contained in the struct
  [[nodiscard]] auto info() -> string {
    string ret;
    ret += fmt::format("Channel ID:{}\n", channel_id);
    ret += fmt::format("Row Index:{}\n", row_index);
    ret += fmt::format("Group ID:{}\n", group_id);
    ret += fmt::format("Electrode Group:{}\n", electrode_group);
    if (label)
      ret += fmt::format("Label:{}\n", label);
    else
      ret += fmt::format("Label:\n");
    if (raw_data_type)
      ret += fmt::format("Raw data Type:{}\n", raw_data_type);
    else
      ret += fmt::format("Raw data Type:\n");
    if (unit)
      ret += fmt::format("Unit:{}\n", unit);
    else
      ret += fmt::format("Unit:\n");
    ret += fmt::format("Exponent:{}\n", exponent);
    ret += fmt::format("AD Zero:{}\n", ad_zero);
    ret += fmt::format("Tick:{}\n", tick);
    ret += fmt::format("Conversion factor:{}\n", conversion_factor);
    ret += fmt::format("ADC bits:{}\n", adc_bits);
    if (high_pass_filter_type)
      ret += fmt::format("HighPass Filter Type:{}\n", high_pass_filter_type);
    else
      ret += fmt::format("HighPass Filter Type:\n");
    if (high_pass_filter_cutoff)
      ret +=
          fmt::format("HighPass Filter CutOff:{}\n", high_pass_filter_cutoff);
    else
      ret += fmt::format("HighPass Filter CutOff:\n");
    ret += fmt::format("HighPass Filter Order:{}\n", high_pass_filter_order);
    if (low_pass_filter_type)
      ret += fmt::format("LowPass Filter Type:{}\n", low_pass_filter_type);
    else
      ret += fmt::format("LowPass Filter Type:\n");
    if (low_pass_filter_cutoff)
      ret += fmt::format("LowPass Filter CutOff:{}\n", low_pass_filter_cutoff);
    else
      ret += fmt::format("LowPass Filter CutOff:\n");
    ret += fmt::format("LowPass Filter Order:{}\n", low_pass_filter_order);
    return ret;
  }
};

/// H5Analog
/// Class to access an analog signal stored in a MultiChannel HDF5 file
///
/// An instance of this class will handle the content of an analog stream
/// stored in an HDF5 file. A stream can hold more than on channel and for each
/// of them will contain two sets of information: on is an element of the
/// InfoChannel dataset inside the group that contains informations on how the
/// data should be interpreted, the other is a raw of the matrix DataChannels
/// and contains the actual data as integer values as recorded from the ADC.
/// This class will help accessing this data reading and converting them on
/// need when queried but will not hold the data itself. For this reason it
/// cannot be copied but only moved so that only one handle to the data can
/// exist.
class H5Analog {
public:
  H5Analog(hid_t group_id, string name, hid_t channel_data);
  ~H5Analog();
  H5Analog(H5Analog &&moved);
  auto operator=(H5Analog &&moved) -> H5Analog;

  H5Analog(const H5Analog &other) = delete;
  auto operator=(const H5Analog &other) = delete;

  [[nodiscard]] auto info() const -> string;
  [[nodiscard]] auto operator[](const string &label) -> Result<Signal>;

private:
  bool moved = false;

  hid_t group_id;
  string name;
  hid_t channel_data;
  unsigned long long n_channels;
  std::unordered_map<string, int> labels_dict;
  vector<info_channel_struct> info_channels;
};

/// H5Content
/// Class to access the content of a HDF5 file with MultiChannel Systems data
///
/// An instance of this class will handle the access to the data stored in a
/// HDF5 file created via MultiChannel Data Manager after the conversion from
/// the recorded data. The instance will open the file and lazy load data when
/// required and close the file after going out of scope. For this reason is
/// not possible to copy an instance but only move it, so that the file will
/// not be closed ahead of schedule. Moreover an instance of this class is not
/// directly constructable but to access a file a call to the static method
/// Open is needed and also a check on its return type that could be an
/// instance of H5Content or a nullopt if an error occurred.
/// In an HDF5 file MultiChannel's software could have stored a number of data
/// not all of which are already managed. The actual goal is to access the raw
/// signals stored in it that could represent the recording of an electrode or
/// some sort of sync signal for analize a stimulation; those values can be
/// found in the H5Analog elements of the analogs vector attribute of this
/// class. The order of the storage of this data (first the stimulation signal,
/// then the raw recording) is not guaranteed.
class H5Content {
public:
  static auto Open(string filename) -> Result<H5Content>;
  ~H5Content();
  H5Content(H5Content &&moved);
  auto operator=(H5Content &&moved) -> H5Content;

  H5Content(const H5Content &other) = delete;
  auto operator=(const H5Content &other) -> H5Content & = delete;

  [[nodiscard]] auto get_tree() const -> string;

  vector<H5Analog> analogs;

private:
  H5Content(hid_t file_id, hid_t base_group_id);
  bool moved = false;

  hid_t file_id;
  hid_t base_group_id;
};
} // namespace CodePP::HDF5

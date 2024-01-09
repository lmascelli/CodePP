#include <codepp/core/operations/spikes_detection.hpp>
#include <codepp/hdf5/h5export.hpp>
#include <codepp/hdf5/h5import.hpp>
#include <codepp/meas/mea60.hpp>
#include <prelude.hpp>

auto main() -> int {
  using namespace CodePP;
  const string filename =
      "E:/unige/raw data/03-10-2023/34341/hdf5/34341_DIV49_basal_9.h5";
  auto data = unwrap(HDF5::H5Content::Open(filename));
  auto mea60 = unwrap(MEA::Mea60::build("34341"));
  auto recording = unwrap(data.fill_mea(2, mea60));
  auto spikes = unwrap(spike_detection(recording, {10e-5, 2e-3, 2e-3}, 4));

  const string savefile = "test.h5";
  HDF5::H5File file(savefile);
  for (auto &[label, index] : recording.signal_map) {
    auto &group = file.add_struct(format("channel_{}", label));
    // HDF5::H5Signal(group, recording.signals[index]);
    group.add_uints("spikes", spikes[label]);
  }
  return 0;
}

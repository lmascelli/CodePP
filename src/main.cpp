#include <codepp/core/operations/spikes_detection.hpp>
#include <codepp/hdf5/h5export.hpp>
#include <codepp/hdf5/h5import.hpp>
#include <codepp/meas/mea60.hpp>
#include <prelude.hpp>

auto main() -> int {
  using namespace CodePP;
  const string filename =
      "E:/unige/raw data/03-10-2023/34341/hdf5/34341_DIV49_basal_0.h5";

  auto data = unwrap(HDF5::H5Content::Open(filename));

  auto mea60 = unwrap(MEA::Mea60::build());
  HDF5::H5File saved("test.h5");

  for (int i = 1; i < 9; ++i) {
    auto &struct_signal = saved.add_struct(fmt::format("signal{}", i));

    auto signal = unwrap(data.analogs[2][unwrap(mea60.get_electrode(3, i))]);

    auto spikes_detection =
        unwrap(spike_detection(signal, {10e-5, 2e-3, 1e-3}));

    struct_signal.add_uints("spikes", spikes_detection);
    HDF5::H5Signal(struct_signal, signal);
  }
  return 0;
}

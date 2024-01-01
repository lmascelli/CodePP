#pragma once
#include <codepp/core/types/signal.hpp>
#include <fstream>
#include <prelude.hpp>

namespace CodePP::csv {
template <typename T>
auto write_signal(const Signal<T> &signal, const string &filename)
    -> Result<size_t> {

  std::ofstream ofs(filename);
  if (ofs.is_open()) {
    ofs << signal.sampling_frequency;
    for (auto val : signal.data) {
      ofs << "," << val;
    }
    return signal.data.size();
  } else {
    return Error{fmt::format("Cannot open file {}", filename)};
  }
}
} // namespace CodePP::csv

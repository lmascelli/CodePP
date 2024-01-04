#include <codepp/csv/csv.hpp>

namespace CodePP::csv {
auto write_signal(const Signal &signal, const string &filename)
    -> Result<size_t> {

  std::ofstream ofs(filename);
  if (ofs.is_open()) {
    ofs << signal.sampling_frequency;
    for (auto val : signal.get_data()) {
      ofs << "," << val;
    }
    return signal.get_data().size();
  } else {
    return Error{fmt::format("Cannot open file {}", filename)};
  }
}
} // namespace CodePP::csv

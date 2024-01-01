#include <algorithm>
#include <cstdlib>
#include <prelude.hpp>
#include <codepp/meas/mea60.hpp>

namespace CodePP::MEA {
auto Mea60::get_row(const string &label) -> Result<int> {
  char *end;
  auto label_i = std::strtol(label.c_str(), &end, 10);
  if (end == nullptr) {
    return Error{fmt::format("Mea60::get_row not a valid label {}", label)};
  } else {
    return label_i % 10;
  }
  return 0;
}
auto Mea60::get_column(const string &label) -> Result<int> {
  char *end;
  auto label_i = std::strtol(label.c_str(), &end, 10);
  if (end == nullptr) {
    return Error{fmt::format("Mea60::get_column not a valid label {}", label)};
  } else {
    return label_i / 10;
  }
  return 0;
}

auto Mea60::contains(const string &label) -> bool {
  return Mea60::contains(unwrap(Mea60::get_row(label)),
                         unwrap(Mea60::get_column(label)));
}
auto Mea60::contains(int row, int column) -> bool {
  return std::find(begin(Mea60::electrodes), end(Mea60::electrodes),
                   std::pair{static_cast<unsigned int>(row),
                             static_cast<unsigned int>(column)});
}

auto Mea60::get_label(int row, int column) -> Result<const string> {
  return fmt::format("{}{}", row, column);
}

auto Mea60::get_name() -> const string & { return name; }
} // namespace CodePP::MEA

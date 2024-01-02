#include <codepp/meas/mea60.hpp>
#include <prelude.hpp>

namespace CodePP::MEA {
auto Mea60::build(vector<string> grounded) -> Result<Mea> {
  vector<Electrode> electrodes(60);
  std::copy(Mea60::electrodes.begin(), Mea60::electrodes.end(),
            electrodes.begin());
  for (auto &label : grounded) {
    auto el = std::find_if(electrodes.begin(), electrodes.end(),
                           [label](auto ele) { return ele.label == label; });
    if (el == electrodes.end())
      return Error{fmt::format("Mea60::build(vector<string> grounded): the "
                               "label {} is not in the Mea60 electrode labels",
                               label)};
    else
      el->grounded = true;
  }
  return Mea(electrodes);
}

auto Mea60::build(vector<pair<int, int>> grounded) -> Result<Mea> {
  vector<Electrode> electrodes(60);
  std::copy(Mea60::electrodes.begin(), Mea60::electrodes.end(),
            electrodes.begin());
  for (auto &[row, column] : grounded) {
    auto el = std::find_if(electrodes.begin(), electrodes.end(),
                           [row, column](auto ele) {
                             return ele.row == row and ele.column == column;
                           });
    if (el == electrodes.end())
      return Error{fmt::format(
          "Mea60::build(vector<pair<int, int>> grounded): the "
          "position ({}, {}) is not in the Mea60 electrode positions",
          row, column)};
    else
      el->grounded = true;
  }
  return Mea(electrodes);
}
} // namespace CodePP::MEA

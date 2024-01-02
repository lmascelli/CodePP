#include <algorithm>
#include <codepp/core/types/mea.hpp>

namespace CodePP {
Mea::Mea(vector<Electrode> electrodes) : electrodes(std::move(electrodes)) {
  for (auto &el : electrodes) {
    if (not el.grounded)
      active_electrodes.push_back(el);
  }
}

auto Mea::get_electrode(int row, int column) -> Result<const Electrode> {
  auto ret = std::find_if(electrodes.begin(), electrodes.end(),
                          [row, column](Electrode &v) {
                            return (v.row == row && v.column == column);
                          });
  if (ret == electrodes.end()) {
    return Error{fmt::format(
        "Mea::get_electrode: Cannot find electrode with coords ({},{})", row,
        column)};
  } else
    return *ret;
}

auto Mea::get_electrode(const string &label) -> Result<const Electrode> {
  auto ret = std::find_if(electrodes.begin(), electrodes.end(),
                          [label](Electrode &v) { return (v.label == label); });
  if (ret == electrodes.end()) {
    return Error{fmt::format(
        "Mea::get_electrode: Cannot find electrode with label \"{}\"", label)};
  } else
    return *ret;
}

auto Mea::get_electrodes() const -> const vector<Electrode> & {
  return electrodes;
}
} // namespace CodePP

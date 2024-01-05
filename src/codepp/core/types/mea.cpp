#include <algorithm>
#include <codepp/core/types/mea.hpp>

namespace CodePP {
Mea::Mea(const string &id, vector<Electrode> electrodes)
    : id(std::move(id)), electrodes(std::move(electrodes)) {
  for (auto &el : this->electrodes) {
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

auto Mea::get_active_electrodes() const -> const vector<Electrode> & {
  return active_electrodes;
}

auto Mea::ground(const string &label) -> Result<bool> {
  auto el = std::remove_if(active_electrodes.begin(), active_electrodes.end(),
                         [label](Electrode &v) { return (v.label == label); });
  if (el == active_electrodes.end()) {
    return Error{fmt::format(
        "Mea::ground: Cannot find electrode with label \"{}\"", label)};
  }
  return true;
}

auto Mea::ground(int row, int column) -> Result<bool> {
  auto el = std::remove_if(active_electrodes.begin(), active_electrodes.end(),
                         [row, column](Electrode &v) {
                           return (v.row == row && v.column == column);
                         });
  if (el == active_electrodes.end()) {
    return Error{
        fmt::format("Mea::ground: Cannot find electrode with coords ({}, {})",
                    row, column)};
  }
  return true;
}

} // namespace CodePP

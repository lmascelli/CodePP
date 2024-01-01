#pragma once
#include <prelude.hpp>
#include <codepp/core/types/mea.hpp>

namespace CodePP {

template <MEA_type Mea> class Electrode {
public:
  auto build(int row, int column) -> Result<Electrode> {
    if (not Mea::contains(row, column)) {
      return Error{fmt::format("Elecrode build: there is no electrode in "
                               "position ({}, {}) present in {}",
                               row, column, Mea::get_name())};
    } else
      return Electrode(row, column);
  }

  auto build(const string &label) -> Result<Electrode> {
    if (not Mea::contains(label)) {
      return Error{fmt::format("Elecrode build: label {} is not present in {}",
                               label, Mea::get_name())};
    } else {
      return Electrode(unwrap(Mea::get_row(label)),
                       unwrap(Mea::get_column(label)));
    }
  }

private:
  Electrode(int row, int column) : row(row), column(column) {}
  int row, column;
};
} // namespace CodePP

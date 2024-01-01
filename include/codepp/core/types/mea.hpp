#pragma once
#include <prelude.hpp>

namespace CodePP {

template <typename M>
concept MEA_type =
    requires(const M &m, const string &label, Result<int> row,
             Result<int> column, bool contained, const string &name) {
      m = M::build();
      row = M::get_row(label);
      column = M::get_column(label);
      contained = M::contains(label);
      contained = M::contains(row, column);
      M::get_label(row, column);
      name = M::get_name();
    };
}

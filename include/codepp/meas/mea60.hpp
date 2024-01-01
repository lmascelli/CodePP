#pragma once
#include <codepp/core/types/electrode.hpp>
#include <codepp/core/types/mea.hpp>

namespace CodePP::MEA {
class Mea60 {
public:
  static auto get_row(const string &label) -> Result<int>;
  static auto get_column(const string &label) -> Result<int>;
  static auto contains(const string &label) -> bool;
  static auto contains(int row, int column) -> bool;
  static auto get_label(int row, int column) -> Result<const string>;
  static auto get_name() -> const string &;

private:
  // clang-format off
  static constexpr array<std::pair<unsigned, unsigned>, 60> electrodes = {{
            {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6}, {1, 7},
    {2, 1}, {2, 2}, {2, 3}, {2, 4}, {2, 5}, {2, 6}, {2, 7}, {2, 8},
    {3, 1}, {3, 2}, {3, 3}, {3, 4}, {3, 5}, {3, 6}, {3, 7}, {3, 8},
    {4, 1}, {4, 2}, {4, 3}, {4, 4}, {4, 5}, {4, 6}, {4, 7}, {4, 8},
    {5, 1}, {5, 2}, {5, 3}, {5, 4}, {5, 5}, {5, 6}, {5, 7}, {5, 8},
    {6, 1}, {6, 2}, {6, 3}, {6, 4}, {6, 5}, {6, 6}, {6, 7}, {6, 8},
    {7, 1}, {7, 2}, {7, 3}, {7, 4}, {7, 5}, {7, 6}, {7, 7}, {7, 8},
            {8, 2}, {8, 3}, {8, 4}, {8, 5}, {8, 6}, {8, 7},
  }}; // clang-format on

  static constexpr string name = "MEA60";
};
} // namespace CodePP::MEA

#pragma once
#include <codepp/core/types/electrode.hpp>
#include <codepp/core/types/mea.hpp>

namespace CodePP::MEA {
class Mea60 {
public:
  Mea60() = delete;
  static auto build(vector<string> grounded) -> Result<Mea>;
  static auto build(vector<pair<int, int>> grounded) -> Result<Mea>;

private:
  // clang-format off
  static constexpr array<Electrode, 60> electrodes = {{
                               {false, 1, 2, "12", 0, 0}, {false, 1, 3, "13", 0, 0}, {false, 1, 4, "14", 0, 0}, {false, 1, 5, "15", 0, 0}, {false, 1, 6, "16", 0, 0}, {false, 1, 7, "17", 0, 0},
    {false, 2, 1, "21", 0, 0}, {false, 2, 2, "22", 0, 0}, {false, 2, 3, "23", 0, 0}, {false, 2, 4, "24", 0, 0}, {false, 2, 5, "25", 0, 0}, {false, 2, 6, "26", 0, 0}, {false, 2, 7, "27", 0, 0}, {false, 2, 8, "28", 0, 0},
    {false, 3, 1, "31", 0, 0}, {false, 3, 2, "32", 0, 0}, {false, 3, 3, "33", 0, 0}, {false, 3, 4, "34", 0, 0}, {false, 3, 5, "35", 0, 0}, {false, 3, 6, "36", 0, 0}, {false, 3, 7, "37", 0, 0}, {false, 3, 8, "38", 0, 0},
    {false, 4, 1, "41", 0, 0}, {false, 4, 2, "42", 0, 0}, {false, 4, 3, "43", 0, 0}, {false, 4, 4, "44", 0, 0}, {false, 4, 5, "45", 0, 0}, {false, 4, 6, "46", 0, 0}, {false, 4, 7, "47", 0, 0}, {false, 4, 8, "48", 0, 0},
    {false, 5, 1, "51", 0, 0}, {false, 5, 2, "52", 0, 0}, {false, 5, 3, "53", 0, 0}, {false, 5, 4, "54", 0, 0}, {false, 5, 5, "55", 0, 0}, {false, 5, 6, "56", 0, 0}, {false, 5, 7, "57", 0, 0}, {false, 5, 8, "58", 0, 0},
    {false, 6, 1, "61", 0, 0}, {false, 6, 2, "62", 0, 0}, {false, 6, 3, "63", 0, 0}, {false, 6, 4, "64", 0, 0}, {false, 6, 5, "65", 0, 0}, {false, 6, 6, "66", 0, 0}, {false, 6, 7, "67", 0, 0}, {false, 6, 8, "68", 0, 0},
    {false, 7, 1, "71", 0, 0}, {false, 7, 2, "72", 0, 0}, {false, 7, 3, "73", 0, 0}, {false, 7, 4, "74", 0, 0}, {false, 7, 5, "75", 0, 0}, {false, 7, 6, "76", 0, 0}, {false, 7, 7, "77", 0, 0}, {false, 7, 8, "78", 0, 0},
                               {false, 8, 2, "82", 0, 0}, {false, 8, 3, "83", 0, 0}, {false, 8, 4, "84", 0, 0}, {false, 8, 5, "85", 0, 0}, {false, 8, 6, "86", 0, 0}, {false, 8, 7, "87", 0, 0},
  }}; // clang-format on

  static constexpr string name = "MEA60";
};
} // namespace CodePP::MEA

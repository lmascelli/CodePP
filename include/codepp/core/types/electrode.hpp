#pragma once
#include <prelude.hpp>

namespace CodePP {
struct Electrode {
  // status data
  bool grounded;

  // refererring name data
  int row, column;
  string label;

  // geometry data
  float posX, posY;
};
} // namespace CodePP

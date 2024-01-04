#pragma once
#include <functional>
#include <prelude.hpp>

namespace CodePP {

using sample = size_t;

enum UNIT {
  UNKNOWN = -1,
  VOLT,
};

class Signal {
public:
  Signal(bool holds_data = true);
  Signal(const Signal &copied) = delete;
  auto operator=(const Signal &copied) = delete;
  Signal(Signal &&moved);
  auto operator=(Signal &&moved) -> Signal;
  [[nodiscard]] auto copy() const -> Signal;

  [[nodiscard]] auto get_data() const -> std::span<const float>;
  [[nodiscard]] auto get_held_data()
      -> Result<std::reference_wrapper<vector<float>>>;
  [[nodiscard]] auto operator()(unsigned long long start, long long end = 0)
      -> Result<Signal>;
  // for absissa conversion from sample number to time
  float sampling_frequency;

  // unit measure data
  UNIT unit;
  int scale;

private:
  bool moved = false;
  bool holds_data = true;

  // actual data
  vector<float> data;

  // slice data;
  std::span<float> slice;
};
}; // namespace CodePP

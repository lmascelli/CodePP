#pragma once
#include <prelude.hpp>

namespace CodePP {

using sample = unsigned int;

enum UNIT {
  UNKNOWN = -1,
  VOLT,
};

template <typename T> class Signal {
public:
  Signal() { sampling_frequency = 0; }
  Signal(const Signal &copied) = delete;
  auto operator=(const Signal &copied) = delete;
  Signal(Signal &&moved) {
    moved.moved = true;
    sampling_frequency = moved.sampling_frequency;
    data = std::move(moved.data);
  }
  auto operator=(Signal &&moved) -> Signal {
    moved.moved = true;
    Signal ret;
    ret.sampling_frequency = moved.sampling_frequency;
    ret.data = std::move(moved.data);
    return ret;
  }
  [[nodiscard]] auto copy() const -> Signal {
    Signal ret;
    ret.sampling_frequency = sampling_frequency;
    std::ranges::copy(data, begin(ret.data));
    return ret;
  }

  // actual data
  vector<T> data;

  // for absissa conversion from sample number to time
  float sampling_frequency;

  // unit measure data
  UNIT unit;
  int scale;

private:
  bool moved = false;
};

using SignalF = Signal<float>;
}; // namespace CodePP

#pragma once
#include <codepp/core/types/signal.hpp>

namespace CodePP::Math {

template <typename T> inline auto abs(T val) -> T {
  return val >= 0 ? val : -val;
}
auto mean(const Signal &signal) -> float;
auto std(const Signal &signal) -> float;
} // namespace CodePP::Math

#include <codepp/core/operations/math.hpp>
#include <numeric>

namespace CodePP::Math {

auto mean(const Signal &signal) -> float {
  auto data = signal.get_data();
  auto sum = std::accumulate(data.begin(), data.end(), 0);
  return sum / (float)(data.size());
}

auto std(const Signal &signal) -> float {
  auto data = signal.get_data();
  auto mean = Math::mean(signal);
  auto sum = std::accumulate(data.begin(), data.end(), 0,
                             [mean](float acc, const float &el) {
                               return acc + (el - mean) * (el - mean);
                             });
  return sum / (float)(data.size());
}
} // namespace CodePP::Math

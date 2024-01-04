#include <codepp/core/types/signal.hpp>
#include <functional>

namespace CodePP {

Signal::Signal(bool holds_data) : holds_data(holds_data) {
  sampling_frequency = 0;
}

Signal::Signal(Signal &&moved) {
  moved.moved = true;
  sampling_frequency = moved.sampling_frequency;
  if (holds_data)
    data = std::move(moved.data);
  else
    slice = std::move(moved.slice);
}

auto Signal::operator=(Signal &&moved) -> Signal {
  moved.moved = true;
  Signal ret;
  ret.sampling_frequency = moved.sampling_frequency;
  if (moved.holds_data)
    ret.data = std::move(moved.data);
  else
    ret.slice = std::move(moved.slice);
  return ret;
}

auto Signal::copy() const -> Signal {
  Signal ret;
  ret.sampling_frequency = sampling_frequency;
  std::ranges::copy(get_data(), begin(ret.data));
  return ret;
}

auto Signal::get_data() const -> std::span<const float> {
  if (holds_data) {
    return {data.data(), data.size()};
  } else {
    return slice;
  }
}

auto Signal::get_held_data() -> Result<std::reference_wrapper<vector<float>>> {
  if (not holds_data)
    return Error{"This signal does not hold any data"};
  else {
    return data;
  }
}

auto Signal::operator()(unsigned long long start, long long end)
    -> Result<Signal> {
  // check on span bounds
  if (start >= data.size())
    return Error{fmt::format(
        "Signal::operator(start, end): start {} excedes data length {}", start,
        data.size())};

  if ((size_t)std::abs(end) > data.size())
    return Error{fmt::format(
        "Signal::operator(start, end): end {} excedes data length {}", start,
        data.size())};

  if (end > 0 and (unsigned long long) end < start)
    return Error{
        fmt::format("Signal::operator(start, end): end {} must follow start {}",
                    end, start)};

  if (end < 0 and (data.size() - end < start))
    return Error{fmt::format(
        "Signal::operator(start, end): end ({}-{}) must follow start {}",
        data.size(), end, start)};

  Signal ret(false);

  if (end == 0) {
    ret.slice = std::span(data.begin() + start, data.end());
  } else if (end > 0) {
    ret.slice = std::span(data.begin() + start, data.begin() + end);
  } else {
    ret.slice = std::span(data.begin() + start, data.end() - end);
  }
  return ret;
}
} // namespace CodePP

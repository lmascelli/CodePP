#pragma once

#include "codepp/core/types/signal.hpp"
namespace CodePP {

struct SpikeDetectionParameters {
  float threshold_V;
  float peak_duration;
  float refractary_time;
};

[[nodiscard]] auto spike_detection(Signal &signal, SpikeDetectionParameters sdp)
    -> Result<vector<sample>>;

[[nodiscard]] auto compute_threshold(Signal signal,
                                     float multiplier_coefficient = 8)
    -> Result<float>;
} // namespace CodePP

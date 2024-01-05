#pragma once
#include "codepp/core/types/recording.hpp"
#include "codepp/core/types/signal.hpp"
#include <unordered_map>

namespace CodePP {

struct SpikeDetectionParameters {
  float threshold_V;
  float peak_duration;
  float refractary_time;
};

[[nodiscard]] auto spike_detection(Signal &signal, const SpikeDetectionParameters& sdp)
    -> Result<vector<sample>>;

[[nodiscard]] auto spike_detection(Recording &recording,
                                   const SpikeDetectionParameters& sdp, unsigned int threads=1)
    -> Result<std::unordered_map<string, vector<sample>>>;

[[nodiscard]] auto compute_threshold(Signal signal,
                                     float multiplier_coefficient = 8)
    -> Result<float>;
} // namespace CodePP

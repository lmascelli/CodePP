#pragma once

#include "codepp/core/types/signal.hpp"
namespace CodePP {

struct SpikeDetectionParameters {};

[[nodiscard]] auto spike_detection(Signal<float> signal,
                                   SpikeDetectionParameters sdp)
    -> vector<sample>;
} // namespace CodePP

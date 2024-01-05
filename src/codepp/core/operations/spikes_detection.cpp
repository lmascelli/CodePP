#include "codepp/core/types/electrode.hpp"
#include <codepp/core/operations/math.hpp>
#include <codepp/core/operations/spikes_detection.hpp>
#include <thread>
#include <unordered_map>

namespace CodePP {

static const sample OVERLAP = 5;

auto spike_detection(Signal &signal, const SpikeDetectionParameters &sdp)
    -> Result<vector<sample>> {
  auto data = signal.get_data();
  sample peak_duration = sdp.peak_duration * signal.sampling_frequency;
  sample refractary_time = sdp.refractary_time * signal.sampling_frequency;

  const size_t data_length = data.size();
  // checks on data_length
  if (data_length < 2 or data_length < peak_duration)
    return Error{fmt::format("spike_detection: Signal has lenght {} that is "
                             "less than peakDuration that has length",
                             data_length, peak_duration)};

  sample interval, index, in_interval_index, new_index = 1;
  sample peak_start_sample, peak_end_sample;
  float peak_start_value, peak_end_value;

  vector<sample> ret;
  ret.reserve(data_length * 0.01);

  for (index = 2; index < data_length; ++index) {
    if (index < new_index)
      continue;
    if ((Math::abs(data[index]) > Math::abs(data[index - 1])) and
        (Math::abs(data[index]) >= Math::abs(data[index + 1]))) {

      // check if the end of the interval where to check for a spike excedes
      // the length of the signal and, eventually, set the interval to end
      // earlier.
      if ((index + peak_duration) > data_length)
        interval = data_length - index;
      else
        interval = peak_duration;

      // temporarely set the start of the spike to be at the current index
      peak_start_sample = index;
      peak_start_value = data[index];

      // look for minimum if the start value of the peak is positive
      if (peak_start_value > 0) {
        peak_end_sample = index + 1;
        peak_end_value = peak_start_value;

        // find the minimum
        for (in_interval_index = (index + 1);
             in_interval_index <= (index + interval); ++in_interval_index) {
          if (data[in_interval_index] < peak_end_value) {
            peak_end_sample = in_interval_index;
            peak_end_value = data[in_interval_index];
          }
        }

        // find the maximum in the interval before the minimum
        for (in_interval_index = (index + 1);
             in_interval_index < peak_end_sample; ++in_interval_index) {
          if (data[in_interval_index] > peak_start_value) {
            peak_start_sample = in_interval_index;
            peak_start_value = data[in_interval_index];
          }
        }

        if ((peak_end_sample == (index + interval)) and
            ((index + interval + OVERLAP) < data_length)) {
          for (sample i = (peak_end_sample + 1);
               i <= (index + interval + OVERLAP); ++i) {
            if (data[i] < peak_end_value) {
              peak_end_sample = i;
              peak_end_value = data[i];
            }
          }
        }
      } // end maximum branch
      // else look for a maximum
      else {
        peak_end_sample = index + 1;
        peak_end_value = peak_start_value;

        // find the maximum
        for (in_interval_index = (index + 1);
             in_interval_index <= (index + interval); ++in_interval_index) {
          if (data[in_interval_index] > peak_end_value) {
            peak_end_sample = in_interval_index;
            peak_end_value = data[in_interval_index];
          }
        }

        // find the minimum in the interval before the minimum
        for (in_interval_index = (index + 1);
             in_interval_index < peak_end_sample; ++in_interval_index) {
          if (data[in_interval_index] < peak_start_value) {
            peak_start_sample = in_interval_index;
            peak_start_value = data[in_interval_index];
          }
        }

        if ((peak_end_sample == (index + interval)) and
            ((index + interval + OVERLAP) < data_length)) {
          for (sample i = (peak_end_sample + 1);
               i <= (index + interval + OVERLAP); ++i) {
            if (data[i] > peak_end_value) {
              peak_end_sample = i;
              peak_end_value = data[i];
            }
          }
        }
      } // end minimum branch

      // check if the difference overtakes the threshold
      auto difference = peak_start_value - peak_end_value;
      if (Math::abs(difference) >= sdp.threshold_V) {
        // store the sample of the high value of the peak
        sample last_peak;
        if (difference > 0)
          last_peak = peak_start_sample;
        else
          last_peak = peak_end_sample;
        ret.push_back(last_peak);

        // set the new index where to start looking for a peak
        if (((last_peak + refractary_time)) > peak_end_sample and
            (last_peak + refractary_time < data_length)) {
          new_index = last_peak + refractary_time;
        } else {
          new_index = peak_end_value + 1;
        }
      }
    }
  }

  return ret;
}

void spike_detection_thread(std::unordered_map<string, vector<sample>> &ret,
                            Recording &recording,
                            const SpikeDetectionParameters &sdp, size_t start,
                            size_t number) {
  for (size_t i = start; i < start + number; ++i) {
    auto &el = recording.mea.get_electrodes()[i];
    auto &signal = recording.signals[recording.signal_map[el.label]];
    ret.insert({el.label, unwrap(spike_detection(signal, sdp))});
  }
}

auto spike_detection(Recording &recording, const SpikeDetectionParameters &sdp,
                     unsigned int n_threads)
    -> Result<std::unordered_map<string, vector<sample>>> {
  std::unordered_map<string, vector<sample>> ret;
  if (n_threads > 1) {
    size_t number_of_electrodes = recording.mea.get_electrodes().size();
    size_t electrodes_per_thread = number_of_electrodes / n_threads;

    vector<std::thread> threads;

    for (size_t i = 0; i < n_threads; ++i) {
      auto number_of_electrodes_to_compute =
          (i == n_threads - 1)
              ? (number_of_electrodes - electrodes_per_thread * (n_threads - 1))
              : electrodes_per_thread;
      threads.emplace_back(spike_detection_thread, std::ref(ret),
                           std::ref(recording), std::ref(sdp),
                           i * electrodes_per_thread,
                           number_of_electrodes_to_compute);
    }
    for (auto &thread : threads)
      thread.join();

    // return Error{"still not implemented"};
  } else {
    for (auto &el : recording.mea.get_active_electrodes()) {
      ret.insert(
          {el.label,
           unwrap(spike_detection(
               recording.signals[recording.signal_map[el.label]], sdp))});
    }
  }
  return ret;
}

auto compute_threshold(Signal signal, float multiplier_coefficient)
    -> Result<float> {
  const sample data_length = signal.get_data().size();
  const unsigned int number_of_windows = 30;
  float window_duration_time =
      200e-3f; // 200 ms window where to compute the threshold
  const sample window_duration_sample =
      window_duration_time * signal.sampling_frequency;

  if (data_length < (window_duration_sample * number_of_windows))
    return Error{
        fmt::format("compute_threshold: too few samples ({}) to automatically "
                    "compute threshold; needed at least {}",
                    data_length, window_duration_sample * number_of_windows)};

  float threshold = 100.f;
  sample windows_distance = data_length / number_of_windows;
  for (unsigned int i = 0; i < number_of_windows; ++i) {
    auto starting_point = windows_distance * i,
         ending_point = starting_point + window_duration_sample;
    auto new_threshold =
        Math::std(unwrap(signal(starting_point, ending_point)));
    if (new_threshold < threshold)
      threshold = new_threshold;
  }

  return threshold * multiplier_coefficient;
}
} // namespace CodePP

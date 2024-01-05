#pragma once
#include <codepp/core/types/mea.hpp>
#include <codepp/core/types/recording.hpp>
#include <codepp/core/types/signal.hpp>

namespace CodePP {

enum RECORDING_TYPE {
  BASAL,
  STIMULATION,
};

class Recording {
public:
  Recording(const Mea& mea);
  string name;
  const Mea& mea;
  vector<Signal> signals;
  optional<Signal> stimulus;
  std::unordered_map<string, size_t> signal_map;
  RECORDING_TYPE type;
};
} // namespace CodePP

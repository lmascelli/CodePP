#pragma once
#include <codepp/core/types/electrode.hpp>
#include <prelude.hpp>

namespace CodePP {

class ActiveElectrodeIterator {};

class Mea {
public:
  Mea(vector<Electrode> electrodes);
  Mea(const Mea &copied) = delete;
  auto operator=(const Mea &copied) = delete;
  Mea(Mea &&moved) = default;
  auto operator=(Mea &&moved) -> Mea & = default;

  [[nodiscard]] auto get_electrode(int row, int column)
      -> Result<const Electrode>;
  [[nodiscard]] auto get_electrode(const string &column)
      -> Result<const Electrode>;
  [[nodiscard]] auto get_electrodes() const -> const vector<Electrode> &;

private:
  vector<Electrode> electrodes;
  vector<Electrode> active_electrodes;
};
}; // namespace CodePP

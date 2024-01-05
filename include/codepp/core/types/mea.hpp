#pragma once
#include <codepp/core/types/electrode.hpp>
#include <prelude.hpp>

namespace CodePP {

class ActiveElectrodeIterator {};

class Mea {
public:
  Mea(const string& id, vector<Electrode> electrodes);
  Mea(const Mea &copied) = delete;
  auto operator=(const Mea &copied) = delete;
  Mea(Mea &&moved) = default;
  auto operator=(Mea &&moved) -> Mea & = default;

  [[nodiscard]] auto get_electrode(int row, int column)
      -> Result<const Electrode>;
  [[nodiscard]] auto get_electrode(const string &column)
      -> Result<const Electrode>;
  [[nodiscard]] auto get_electrodes() const -> const vector<Electrode> &;
  [[nodiscard]] auto get_active_electrodes() const -> const vector<Electrode> &;
  [[nodiscard]] auto ground(const string& label) -> Result<bool>;
  [[nodiscard]] auto ground(int row, int column) -> Result<bool>;

private:
  string id;
  vector<Electrode> electrodes;
  vector<Electrode> active_electrodes;
};
}; // namespace CodePP

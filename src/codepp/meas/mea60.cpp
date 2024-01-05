#include <codepp/meas/mea60.hpp>
#include <prelude.hpp>

namespace CodePP::MEA {
auto Mea60::build(const string &id) -> Result<Mea> {
  vector<Electrode> electrodes(60);
  std::copy(Mea60::electrodes.begin(), Mea60::electrodes.end(),
            electrodes.begin());
  return Mea(id, electrodes);
}
} // namespace CodePP::MEA

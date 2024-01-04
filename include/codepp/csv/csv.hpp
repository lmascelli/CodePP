#pragma once
#include <codepp/core/types/signal.hpp>
#include <fstream>
#include <prelude.hpp>

namespace CodePP::csv {
auto write_signal(const Signal &signal, const string &filename)
    -> Result<size_t>;
} // namespace CodePP::csv

#include "emulator.hpp"

#include <algorithm>
#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>

namespace emulator {

auto trim_line = [](auto s) {
  for (auto &c : s)
    if (c == '\n' || c == '\r' || c == '\t')
      c = ' ';
  boost::trim(s);
};

auto preprocess_lines(const std::string &assembly) {
  using boost::algorithm::is_any_of;
  using boost::algorithm::split;
  using boost::algorithm::token_compress_on;

  static std::vector<std::string> asm_lines{};

  split(asm_lines, assembly, is_any_of("\n\r"), token_compress_on);
  std::for_each(asm_lines.begin(), asm_lines.end(), trim_line);

  return asm_lines;
}

Emulator::Emulator(const std::string &asm_)
    : asm_lines_(preprocess_lines(asm_)) {}

void Emulator::reload_program(const std::string &asm_) {
  asm_lines_ = preprocess_lines(asm_);
}

} // namespace emulator

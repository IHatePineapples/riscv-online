#include "program.hpp"
#include <bitset>
#include <string>

namespace emulator {

using register_t = std::bitset<32>;

class Emulator final {

  register_t x0;
  register_t x1;
  register_t x2;
  register_t x3;
  register_t x4;
  register_t x5;
  register_t x6;
  register_t x7;
  register_t x8;
  register_t x9;
  register_t x10;
  register_t x11;
  register_t x12;
  register_t x13;
  register_t x14;
  register_t x15;
  register_t x16;
  register_t x17;
  register_t x18;
  register_t x19;
  register_t x20;
  register_t x21;
  register_t x22;
  register_t x23;
  register_t x24;
  register_t x25;
  register_t x26;
  register_t x27;
  register_t x28;
  register_t x29;
  register_t x30;
  register_t x31;

public:
  constexpr Emulator() = default;

  void load_program(const std::string &);

  void parse();
  void step();
  void run();
};

} // namespace emulator

#pragma once

#include <bitset>
#include <string>
#include <vector>

#include "RoL/parse.hpp"

namespace emulation
{

  void run();

  template<int xlen>
  class emulator final
  {

    using reg = std::bitset<xlen>;
    reg x0;
    reg x1;
    reg x2;
    reg x3;
    reg x4;
    reg x5;
    reg x6;
    reg x7;
    reg x8;
    reg x9;
    reg x10;
    reg x11;
    reg x12;
    reg x13;
    reg x14;
    reg x15;
    reg x16;
    reg x17;
    reg x18;
    reg x19;
    reg x20;
    reg x21;
    reg x22;
    reg x23;
    reg x24;
    reg x25;
    reg x26;
    reg x27;
    reg x28;
    reg x29;
    reg x30;
    reg x31;

    std::vector<std::string> asm_lines_;
    parse::symbol_table_t symbols_;
    parse::data_t data_;
    parse::text_t asm_;

  public:
    emulator() = default;

  };

} // namespace emulator

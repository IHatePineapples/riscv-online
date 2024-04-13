#pragma once

#include <bitset>
#include <string>
#include <vector>

#include "RoL/parse.hpp"

namespace emulation
{

  void run();

  template<int xlen>
  struct emulator final
  {

    using reg = std::bitset<xlen>;
    reg zero;
    reg ra;
    reg sp;
    reg gp;
    reg tp;
    reg t0;
    reg t1;
    reg t2;
    reg fp;
    reg s1;
    reg a0;
    reg a1;
    reg a2;
    reg a3;
    reg a4;
    reg a5;
    reg a6;
    reg a7;
    reg s2;
    reg s3;
    reg s4;
    reg s5;
    reg s6;
    reg s7;
    reg s8;
    reg s9;
    reg s10;
    reg s11;
    reg t3;
    reg t4;
    reg t5;
    reg t6;

    reg pc;

    std::vector<std::string> asm_lines_;
    parse::symbol_table_t symbols_;
    parse::data_t data_;
    parse::text_t asm_;

    emulator() = default;

  };

} // namespace emulator

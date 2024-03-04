
#include <any>
#include <cstddef>
#include <map>
#include <string>
#include <tuple>
#include <variant>
#include <vector>
namespace parse {

enum class opc {
  // 'R' FMT
  ADD,
  SUB,
  XOR,
  OR,
  AND,
  SLL,
  SRL,
  SRA,
  SLT,
  SLTU,

  // 'I' FMT
  ADDI,
  XORI,
  ORI,
  ANDI,
  SLLI,
  SRLI,
  SRAI,
  SLTI,
  SLTUI,

  // Load, 'I' FMT
  LB,
  LH,
  LW,
  LBU,
  LHU,

  // 'S' FMT
  SB,
  SH,
  SW,

  // 'B' FMT
  BEQ,
  BNE,
  BLT,
  BGE,
  BLTU,
  BGEU,

  // JALx
  JAL,
  JALR,

  // 'U' FMT
  LUI,
  AUIPC,

  // ENV

  ECALL,
  EBREAK,

  // Used later to make a fake symbol table in a second pass
  SYMBOL

};

enum class reg {
  // Fully qualified names
  x0,
  x1,
  x2,
  x3,
  x4,
  x5,
  x6,
  x7,
  x8,
  x9,
  x10,
  x11,
  x12,
  x13,
  x14,
  x15,
  x16,
  x17,
  x18,
  x19,
  x20,
  x21,
  x22,
  x23,
  x24,
  x25,
  x26,
  x27,
  x28,
  x29,
  x30,
  x31,
  // ABI specified names i.e aliases
  zero = x0,
  ra = x1,
  sp = x2,
  gp = x3,
  tp = x4,
  t0 = x5,
  t1 = x6,
  t2 = x7,
  s0 = x8,
  fp = x8,
  s1 = x9,
  a0 = x10,
  a1 = x11,
  a2 = x12,
  a3 = x13,
  a4 = x14,
  a5 = x15,
  a6 = x16,
  a7 = x17,
  s2 = x18,
  s3 = x19,
  s4 = x20,
  s5 = x21,
  s6 = x22,
  s7 = x23,
  s8 = x24,
  s9 = x25,
  s10 = x26,
  s11 = x27,
  t3 = x28,
  t4 = x29,
  t5 = x30,
  t6 = x31,

  // Padding
  NONE
};

using text_entry_t = std::variant<std::string, std::tuple<opc, reg, reg, reg>>;

using data_t = std::map<std::string, std::any>;
using text_t = std::vector<text_entry_t>;
using symbol_table_t = std::map<std::string, std::size_t>;

static constexpr data_t data(const std::string &program);
static constexpr text_t text(const std::string &program);
static symbol_table_t symbols(const text_t &text) {
  symbol_table_t st = {};
  for (std::size_t i = 0; i < text.size(); ++i) {
    if (!std::holds_alternative<std::string>(text[i]))
      continue;
    st[std::get<std::string>(text[i])] = i;
  }
  return st;
}

} // namespace parse

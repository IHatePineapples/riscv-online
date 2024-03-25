#include <algorithm>
#include <any>
#include <cctype>
#include <cstddef>
#include <map>
#include <optional>
#include <stdexcept>
#include <string>
#include <tuple>
#include <unordered_map>
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

  UNSUPPORTED = -1
};

const static std::unordered_map<std::string_view, opc> s_to_opc{

    {"add", opc::ADD},     {"sub", opc::SUB},     {"xor", opc::XOR},
    {"or", opc::OR},       {"and", opc::AND},     {"sll", opc::SLL},
    {"srl", opc::SRL},     {"sra", opc::SRA},     {"slt", opc::SLT},
    {"sltu", opc::SLTU},   {"addi", opc::ADDI},   {"xori", opc::XORI},
    {"ori", opc::ORI},     {"andi", opc::ANDI},   {"slli", opc::SLLI},
    {"srli", opc::SRLI},   {"srai", opc::SRAI},   {"slti", opc::SLTI},
    {"sltui", opc::SLTUI}, {"sb", opc::SB},       {"sh", opc::SH},
    {"sw", opc::SW},       {"beq", opc::BEQ},     {"bne", opc::BNE},
    {"blt", opc::BLT},     {"bge", opc::BGE},     {"bltu", opc::BLTU},
    {"bgeu", opc::BGEU},   {"lb", opc::LB},       {"lh", opc::LH},
    {"lw", opc::LW},       {"lbu", opc::LBU},     {"lhu", opc::LHU},
    {"jal", opc::JAL},     {"jalr", opc::JALR},   {"lui", opc::LUI},
    {"auipc", opc::AUIPC}, {"ecall", opc::ECALL}, {"ebreak", opc::EBREAK}};

inline bool operator==(const std::string &s, const opc &o) {
  try {
    return s_to_opc.at(s) == o;
  } catch (const std::out_of_range &) {
    return false;
  }
}

static opc to_opc(const std::string_view opc_s) {
  try {
    return s_to_opc.at(opc_s);
  } catch (const std::out_of_range &) {
    return opc::UNSUPPORTED;
  }
}

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
  NONE = -1
};

static std::unordered_map<std::string_view, reg> s_to_reg{
    {"x0", reg::x0},   {"x1", reg::x1},   {"x2", reg::x2},   {"x3", reg::x3},
    {"x4", reg::x4},   {"x5", reg::x5},   {"x6", reg::x6},   {"x7", reg::x7},
    {"x8", reg::x8},   {"x9", reg::x9},   {"x10", reg::x10}, {"x11", reg::x11},
    {"x12", reg::x12}, {"x13", reg::x13}, {"x14", reg::x14}, {"x15", reg::x15},
    {"x16", reg::x16}, {"x17", reg::x17}, {"x18", reg::x18}, {"x19", reg::x19},
    {"x20", reg::x20}, {"x21", reg::x21}, {"x22", reg::x22}, {"x23", reg::x23},
    {"x24", reg::x24}, {"x25", reg::x25}, {"x26", reg::x26}, {"x27", reg::x27},
    {"x28", reg::x28}, {"x29", reg::x29}, {"x30", reg::x30}, {"x31", reg::x31},
    {"zero", reg::x0}, {"ra", reg::x1},   {"sp", reg::x2},   {"gp", reg::x3},
    {"tp", reg::x4},   {"t0", reg::x5},   {"t1", reg::x6},   {"t2", reg::x7},
    {"s0", reg::x8},   {"fp", reg::x8},   {"s1", reg::x9},   {"a0", reg::x10},
    {"a1", reg::x11},  {"a2", reg::x12},  {"a3", reg::x13},  {"a4", reg::x14},
    {"a5", reg::x15},  {"a6", reg::x16},  {"a7", reg::x17},  {"s2", reg::x18},
    {"s3", reg::x19},  {"s4", reg::x20},  {"s5", reg::x21},  {"s6", reg::x22},
    {"s7", reg::x23},  {"s8", reg::x24},  {"s9", reg::x25},  {"s10", reg::x26},
    {"s11", reg::x27}, {"t3", reg::x28},  {"t4", reg::x29},  {"t5", reg::x30},
    {"t6", reg::x31},
};

static reg to_reg(const std::string_view s) {
  try {
    return s_to_reg.at(s);
  } catch (const std::out_of_range &) {
    return reg::NONE;
  }
}

// Aliases
using imm_t = int;
using opa0_t = reg;
using symbol_t = std::string;

// Type-aggregates
using opa1_t = std::variant<reg, imm_t>;
using opa2_t = opa1_t;

using instruction_t = std::tuple<opc, opa0_t, opa1_t, opa2_t>;
using I = std::tuple<opc, reg, reg, imm_t>;
using S = std::tuple<opc, reg, reg, imm_t>;
using SB = std::tuple<opc, reg, reg, imm_t>;
using R = std::tuple<opc, reg, reg, reg>;
using U = std::tuple<opc, reg, imm_t>;

using text_entry_t = std::variant<symbol_t, I, S, SB, R, U>;

using data_t = std::map<std::string, std::any>;
using text_t = std::vector<text_entry_t>;
using symbol_table_t = std::map<std::string, std::size_t>;

static data_t data(const std::vector<std::string> &asm_lines) {
  data_t data = {};
  const auto start = std::find(asm_lines.begin(), asm_lines.end(), ".data:");

  if (start == asm_lines.end())
    return data;

  const auto last = std::find(start + 1, asm_lines.end(), ".text:");
  if (last == asm_lines.end())
    throw std::runtime_error{"No '.text' section!"};

  static auto tokenize = [&data](const std::string &s) {
    static std::size_t cursor = 0;
    if (s.substr(0, 4) == ".equ")
      cursor += 4;

    while (s[cursor] == ' ' || s[cursor] == ',')
      ++cursor;

    auto symbol_start = cursor;
    while (s[cursor] == ',')
      ++cursor;
    auto symbol_size = cursor - symbol_start;

    while (s[cursor] == ' ')
      ++cursor;

    data[s.substr(symbol_start, symbol_size)] =
        std::stoi(s.substr(cursor, s.length() - cursor));
  };

  std::for_each(start + 1, last, tokenize);

  return data;
};

static std::optional<I> make_I(const std::string_view s) {
  static std::size_t cursor = 0;
  while (isalpha(s[cursor]))
    ++cursor;
  static auto opc = to_opc(s.substr(0, cursor));
  while (!isalnum(s.at(cursor)))
    ++cursor;
  static auto opa1_i = cursor;
  while (isalnum(s.at(cursor)))
    ++cursor;
  static auto opa1 = to_reg(s.substr(opa1_i, cursor - opa1_i));
  while (!isalnum(s.at(cursor)))
    ++cursor;
  static auto opa2_i = cursor;
  while (isalnum(s.at(cursor)))
    ++cursor;
  static auto opa2 = to_reg(s.substr(opa2_i, cursor - opa2_i));
  while (!isalnum(s.at(cursor)))
    ++cursor;

  static int imm;
  try {
    imm = std::stoi(std::string(s.substr(cursor, s.length() - cursor)));
  } catch (const std::invalid_argument &) {
    return {};
  } catch (const std::out_of_range &) {
    return {};
  }
  return std::make_tuple(opc, opa1, opa2, imm);
}

static text_t text(const std::vector<std::string> &asm_lines) {
  text_t asm_text = {};

  const auto start = std::find(asm_lines.begin(), asm_lines.end(), ".text:");

  if (start == asm_lines.end())
    return asm_text;

  return asm_text;
};

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

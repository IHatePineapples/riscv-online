#include "emulator.hpp"

#include <algorithm>
#include <string>
#include <vector>

namespace emulation
{

  void run()
  {
    // Nothing yet
  }

  reg &emulator::resolv_rd(const std::bitset<5> &rd)
  {

    switch (rd.to_ulong())
    {
    case 0:
      return zero;
    case 1:
      return ra;
    case 2:
      return sp;
    case 3:
      return gp;
    case 4:
      return tp;
    case 5:
      return t0;
    case 6:
      return t1;
    case 7:
      return t2;
    case 8:
      return fp;
    case 9:
      return s1;
    case 10:
      return a0;
    case 11:
      return a1;
    case 12:
      return a2;
    case 13:
      return a3;
    case 14:
      return a4;
    case 15:
      return a5;
    case 16:
      return a6;
    case 17:
      return a7;
    case 18:
      return s2;
    case 19:
      return s3;
    case 20:
      return s4;
    case 21:
      return s5;
    case 22:
      return s6;
    case 23:
      return s7;
    case 24:
      return s8;
    case 25:
      return s9;
    case 26:
      return s10;
    case 27:
      return s11;
    case 28:
      return t3;
    case 29:
      return t4;
    case 30:
      return t5;
    case 31:
      return t6;
    default:
      printf("%s:%d: Weird RD content, got '%lu'", __PRETTY_FUNCTION__, __LINE__, rd.to_ulong());
      return zero;
    }
  }

  void emulator::inc_pc()
  {
    bool carry = false;
    for (int i = 4; i < 32; ++i)
    {
      if (!pc.test(i))
      {
        pc.set(i, 1);
        carry = false;
      }
      else
      {
        pc.set(i, 0);
        carry = true;
      }

      if (!carry)
        return;
    }

    if (carry)
      printf("%s:%i: Exhausted PC past maximum ('%s'), overflowed! Proceeding anyway.", __PRETTY_FUNCTION__, __LINE__, pc.to_string().c_str());
  }

  void emulator::lui_(reg &rd, const std::bitset<20> &imm)
  {
    rd = (imm.to_ulong() << (xlen - imm.size()));
  };

  void emulator::auipc_(reg &rd, const std::bitset<20> imm)
  {
    rd = 0;

    for (int i = 0; i < 12; ++i)
      pc[i] = 0;

    bool carry = 0;
    for (int i = 12; i < 32; ++i)
    {
      if (pc[i] and imm[i - 12])
      {
        pc[i] = 0 + carry;
        carry = 1;
      }
      else if (pc[i] xor imm[i - 12])
      {
        pc[i] = !carry;
      }
      else
      {
        pc[i] = carry;
        carry = 0;
      }
    }

    rd = pc;
  };

  void emulator::execute()
  {
    using namespace parse;

    const auto i = pc.to_ulong() >> 5;
    if (i > ram.size())
    {
      printf("%s:%i: Bad PC content or bad math! Out of Range!", __PRETTY_FUNCTION__, __LINE__);
      return;
    }

    const auto &r = ram.at(i);
    std::bitset<7> opc_r;

    for (std::size_t i = 0; i < 7; ++i)
      opc_r[i] = r[i];

    std::bitset<3> opc_l;

    for (std::size_t i = 0; i < opc_l.size(); ++i)
      opc_l[i] = r[i + 12];

    if (opc_r == lui_fmt or opc_r == auipc_fmt)
    {
      std::bitset<20> imm;
      for (std::size_t i = 0; i < imm.size(); ++i)
        imm[i] = r[i + 12];

      std::bitset<5> rd_b;
      for (std::size_t i = 0; i < rd_b.size(); ++i)
        rd_b[i] = r[i + 7];

      reg &rd = resolv_rd(rd_b);
      if (opc_r.test(5))
        lui_(rd, imm);
      else
        auipc_(rd, imm);
    }
  }

  void slt_(reg &rd, const reg &rs1, const reg &rs2)
  {
    /** \todo Two's complement here! */
    if (rs1[rs1.size() - 1] xor rs2[rs2.size() - 1])
    {
      rd = rs1[rs1.size() - 1] > rs2[rs2.size() - 1];
      return;
    }

    if (rs1[rs1.size() - 1] and rs2[rs2.size() - 1])
    {
      rd = rs1.to_ulong() > rs2.to_ulong();
      return;
    }

    rd = rs1.to_ulong() < rs2.to_ulong();
    return;
  };

  void sltu_(reg &rd, const reg &rs1, const reg &rs2)
  {
    rd = rs1.to_ulong() > rs2.to_ulong(); // ? 1 : 0;
  };

  void emulator::xor_(reg &rd, const reg &rs1, const reg &rs2)
  {
    rd = rs1 ^ rs2;
  };

  void emulator::srl_(reg &rd, const reg &rs1, const reg &rs2)
  {
    rd = rs1 >> rs2.to_ulong();
  };

  void emulator::sra_(reg &rd, const reg &rs1, const reg &rs2)
  {
    bool signbit = rs1[rs1.size() - 1];
    rd = rs1 >> rs2.to_ulong();
    rd[rd.size() - 1] = signbit;
  };

  void emulator::or_(reg &rd, const reg &rs1, const reg &rs2)
  {
    rd = rs1 | rs2;
  };

  void emulator::and_(reg &rd, const reg &rs1, const reg &rs2)
  {
    rd = rs1 & rs2;
  };
} // namespace emulator

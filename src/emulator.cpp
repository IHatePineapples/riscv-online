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
  template <std::size_t n = 11>
  static constexpr reg sign_extend(std::bitset<n> imm, std::size_t msb_pos = (n - 1))
  {
    reg tmp = 0;
    for (std::size_t i = 0; i < n; ++i)
      tmp[i] = imm[i];

    // Sign extend
    for (int i = n - 1; i < xlen - 1; ++i)
      tmp[i] = imm[msb_pos];
    return tmp;
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

  void emulator::jal_(reg &rd, const std::bitset<20> imm){

    int offset = 0;
    for (std::size_t i = 0; i < imm.size() - 2; ++i)
      offset += imm[i] << i;

    offset -= imm[imm.size() - 1] << (imm.size() - 1);
    offset <<= 1;
    rd = pc.to_ulong() + (2<<4);
    pc = pc.to_ulong() + offset;
  };
  void emulator::jalr_(reg &rd, reg &rs1, std::bitset<12> imm){

    int offset = 0;
    for (std::size_t i = 0; i < imm.size() - 2; ++i)
      offset += imm[i] << i;

    offset -= imm[imm.size() - 1] << imm.size() - 1;
    offset <<= 1;

    unsigned int address = rs1.to_ulong() + offset;

    rd = pc.to_ulong() + (1<<5);
    pc = address;
  };

  void emulator::beq_(reg &rs1, reg &rs2, std::bitset<12> imm)
  {

    int l = 0;
    for (std::size_t i = 0; i < rs1.size() - 2; ++i)
      l += rs1[i] << i;

    l -= rs1[rs1.size() - 1] << rs1.size() - 1;

    int r = 0;
    for (std::size_t i = 0; i < rs2.size() - 2; ++i)
      r += rs2[i] << i;

    r -= rs2[rs2.size() - 1] << rs2.size() - 1;

    if (l == r)
      return;

    int offset = 0;
    for (std::size_t i = 0; i < imm.size() - 2; ++i)
      offset += imm[i] << i;

    offset -= imm[imm.size() - 1] << imm.size() - 1;

    pc = pc.to_ulong() + (offset << 1);
  }
  void emulator::bne_(reg &rs1, reg &rs2, std::bitset<12> imm)
  {
    int l = 0;
    for (std::size_t i = 0; i < rs1.size() - 2; ++i)
      l += rs1[i] << i;

    l -= rs1[rs1.size() - 1] << rs1.size() - 1;

    int r = 0;
    for (std::size_t i = 0; i < rs2.size() - 2; ++i)
      r += rs2[i] << i;

    r -= rs2[rs2.size() - 1] << rs2.size() - 1;

    if (l == r)
      return;

    int offset = 0;
    for (std::size_t i = 0; i < imm.size() - 2; ++i)
      offset += imm[i] << i;

    offset -= imm[imm.size() - 1] << imm.size() - 1;

    pc = pc.to_ulong() + (offset << 1);
  }

  void emulator::blt_(reg &rs1, reg &rs2, std::bitset<12> imm)
  {
    int l = 0;
    for (std::size_t i = 0; i < rs1.size() - 2; ++i)
      l += rs1[i] << i;

    l -= rs1[rs1.size() - 1] << rs1.size() - 1;

    int r = 0;
    for (std::size_t i = 0; i < rs2.size() - 2; ++i)
      r += rs2[i] << i;

    r -= rs2[rs2.size() - 1] << rs2.size() - 1;

    if (l >= r)
      return;

    int offset = 0;
    for (std::size_t i = 0; i < imm.size() - 2; ++i)
      offset += imm[i] << i;

    offset -= imm[imm.size() - 1] << imm.size() - 1;

    pc = pc.to_ulong() + (offset << 1);
  }
  void emulator::bge_(reg &rs1, reg &rs2, std::bitset<12> imm)
  {
    int l = 0;
    for (std::size_t i = 0; i < rs1.size() - 2; ++i)
      l += rs1[i] << i;

    l -= rs1[rs1.size() - 1] << rs1.size() - 1;

    int r = 0;
    for (std::size_t i = 0; i < rs2.size() - 2; ++i)
      r += rs2[i] << i;

    r -= rs2[rs2.size() - 1] << rs2.size() - 1;

    if (l < r)
      return;

    int offset = 0;
    for (std::size_t i = 0; i < imm.size() - 2; ++i)
      offset += imm[i] << i;

    offset -= imm[imm.size() - 1] << imm.size() - 1;

    pc = pc.to_ulong() + (offset << 1);
  }
  void emulator::bltu_(reg &rs1, reg &rs2, std::bitset<12> imm)
  {
    if (rs1.to_ulong() >= rs2.to_ulong())
      return;

    int offset = 0;
    for (std::size_t i = 0; i < imm.size() - 2; ++i)
      offset += imm[i] << i;

    offset -= imm[imm.size() - 1] << imm.size() - 1;

    pc = pc.to_ulong() + (offset << 1);
  }

  void emulator::bgeu_(reg &rs1, reg &rs2, std::bitset<12> imm)
  {
    if (rs1.to_ulong() < rs2.to_ulong())
      return;
    int offset = 0;
    for (std::size_t i = 0; i < imm.size() - 2; ++i)
      offset += imm[i] << i;

    offset -= imm[imm.size() - 1] << imm.size() - 1;

    pc = pc.to_ulong() + (offset << 1);
  }

  void emulator::lb_(reg &rd, reg &rs1, std::bitset<11> imm)
  {
    int offset = 0;
    for (std::size_t i = 0; i < imm.size() - 2; ++i)
      offset += imm[i] << i;

    offset -= imm[imm.size() - 1] << imm.size() - 1;

    const auto addr = rs1.to_ulong() + offset;
    for (std::size_t i = 0; i < xlen / 4; ++i)
      rd[i] = ram[addr + i];

    rd = sign_extend<xlen>(rd, xlen / 4 - 1);
  };

  void emulator::lh_(reg &rd, reg &rs1, std::bitset<11> imm)
  {
    int offset = 0;
    for (std::size_t i = 0; i < imm.size() - 2; ++i)
      offset += imm[i] << i;

    offset -= imm[imm.size() - 1] << imm.size() - 1;

    const auto addr = rs1.to_ulong() + offset;
    for (std::size_t i = 0; i < xlen / 2; ++i)
      rd[i] = ram[addr + i];

    rd = sign_extend<xlen>(rd, xlen / 2 - 1);
  };

  void emulator::lw_(reg &rd, reg &rs1, std::bitset<11> imm)
  {
    int offset = 0;
    for (std::size_t i = 0; i < imm.size() - 2; ++i)
      offset += imm[i] << i;

    offset -= imm[imm.size() - 1] << imm.size() - 1;

    const auto addr = rs1.to_ulong() + offset;
    for (std::size_t i = 0; i < xlen; ++i)
      rd[i] = ram[addr + i];
  };

  void emulator::lbu_(reg &rd, reg &rs1, std::bitset<11> imm)
  {
    rd = 0;

    int offset = 0;
    for (std::size_t i = 0; i < imm.size() - 2; ++i)
      offset += imm[i] << i;

    offset -= imm[imm.size() - 1] << imm.size() - 1;

    const auto addr = rs1.to_ulong() + offset;
    for (std::size_t i = 0; i < xlen / 4; ++i)
      rd[i] = ram[addr + i];
  };

  void emulator::lhu_(reg &rd, reg &rs1, std::bitset<11> imm)
  {
    rd = 0;

    int offset = 0;
    for (std::size_t i = 0; i < imm.size() - 2; ++i)
      offset += imm[i] << i;

    offset -= imm[imm.size() - 1] << imm.size() - 1;

    const auto addr = rs1.to_ulong() + offset;
    for (int i = 0; i < xlen / 2; ++i)
      rd[i] = ram[addr + i];
  };

  /** \todo Implement real RAM! */
  void emulator::sb_(reg &rs1, reg &rs2, std::bitset<12> imm)
  {
    // RAM not implemented yet !
    // This is will just cause stack corrution/decay lol
    int offset = 0;
    for (std::size_t i = 0; i < imm.size() - 2; ++i)
      offset += imm[i] << i;

    offset -= imm[imm.size() - 1] << imm.size() - 1;

    const auto addr = rs1.to_ulong() + offset;
    for (int i = 0; i < xlen / 4; ++i)
      ram[addr + i] = rs2[i];
  };
  /** \todo Implement real RAM! */
  void emulator::sh_(reg &rs1, reg &rs2, std::bitset<12> imm)
  {
    // RAM not implemented yet !
    // This is will just cause stack corrution/decay lol
    int offset = 0;
    for (std::size_t i = 0; i < imm.size() - 2; ++i)
      offset += imm[i] << i;

    offset -= imm[imm.size() - 1] << imm.size() - 1;

    const auto addr = rs1.to_ulong() + offset;
    for (int i = 0; i < xlen / 2; ++i)
      ram[addr + i] = rs2[i];
  };

  /** \todo Implement real RAM! */
  void emulator::sw_(reg &rs1, reg &rs2, std::bitset<12> imm)
  {
    // RAM not implemented yet !
    // This is will just cause stack corrution/decay lol
    int offset = 0;
    for (std::size_t i = 0; i < imm.size() - 2; ++i)
      offset += imm[i] << i;

    offset -= imm[imm.size() - 1] << imm.size() - 1;
    const auto addr = rs1.to_ulong() + offset;
    for (int i = 0; i < xlen; ++i)
      ram[addr + i] = rs2[i];
  };

  void emulator::addi_(reg &rd, reg &rs1, std::bitset<11> imm)
  {
    add_(rd, rs1, sign_extend(imm));
  };

  void emulator::slti_(reg &rd, reg &rs1, std::bitset<11> imm)
  {
    slt_(rd, rs1, sign_extend(imm));
  };

  void emulator::sltiu_(reg &rd, reg &rs1, std::bitset<11> imm)
  {
    rd = rs1.to_ulong() < sign_extend(imm).to_ulong();
  };

  void emulator::xori_(reg &rd, reg &rs1, std::bitset<11> imm)
  {
    rd = rs1 ^ sign_extend(imm);
  };

  void emulator::ori_(reg &rd, reg &rs1, std::bitset<11> imm)
  {
    rd = rs1 | sign_extend(imm);
  };

  void emulator::andi_(reg &rd, reg &rs1, std::bitset<11> imm)
  {
    rd = rs1 & sign_extend(imm);
  };

  void emulator::slli_(reg &rd, reg &rs1, std::bitset<5> shamt)
  {
    rd = rs1 << shamt.to_ulong();
  };

  void emulator::srli_(reg &rd, reg &rs1, std::bitset<5> shamt)
  {
    rd = rs1 >> shamt.to_ulong();
  };

  void emulator::srai_(reg &rd, reg &rs1, std::bitset<5> shamt)
  {
    rd = rs1 >> shamt.to_ulong();
    rd[rd.size() - 1] = rs1[rs1.size() - 1];
  };

  void emulator::add_(reg &rd, const reg &rs1, const reg &rs2)
  {

    bool carry = 0;
    for (int i = 0; i < 32; ++i)
    {
      if (rs1[i] and rs2[i])
      {
        rd[i] = 0 + carry;
        carry = 1;
      }
      else if (rs1[i] xor rs2[i])
      {
        rd[i] = !carry;
      }
      else
      {
        rd[i] = carry;
        carry = 0;
      }
    }
  };

  void emulator::sub_(reg &rd, const reg &rs1, const reg &rs2)
  {
    // Compute 2's complement, then do an ADD.
    // This is better than duplicating code.

    reg rs2_tmp = rs2;
    rs2_tmp.flip();

    bool carry = 0;
    for (int i = 0; i < 32; ++i)
    {
      if (!rs2_tmp[i])
      {
        rs2_tmp[i] = 1;
        carry = 0;
      }
      else
      {
        rs2_tmp[i] = 0;
        carry = 1;
      }

      if (!carry)
        break;
    }

    emulator::add_(std::forward<reg &>(rd), std::forward<const reg &>(rs1), std::move(rs2_tmp));
  };

  void emulator::slt_(reg &rd, const reg &rs1, const reg &rs2)
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

  void emulator::sltu_(reg &rd, const reg &rs1, const reg &rs2)
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
    rd = rs1 >> rs2.to_ulong();
    rd[rd.size() - 1] = rs1[rs1.size() - 1];
  };

  void emulator::or_(reg &rd, const reg &rs1, const reg &rs2)
  {
    rd = rs1 | rs2;
  };

  void emulator::and_(reg &rd, const reg &rs1, const reg &rs2)
  {
    rd = rs1 & rs2;
  };

  void emulator::execute()
  {
    using namespace parse;

    reg r = 0;

    for (int i = 0; i < 32; ++i)
    {
      r[i] = ram[pc.to_ulong() + i];
    }

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

} // namespace emulator

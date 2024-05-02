#include "RoL/emulation/emulator.hpp"
#include "RoL/parse.hpp"

#include <ranges>
#include <string>
#include <sstream>
#include <vector>

#if !RoL_testing

#include "RoL/threaded/shared.hpp"

#endif
namespace emulation
{

#if !RoL_testing
  void run()
  {
  start_run:
    printf("%s:%d: No jobs, taking a nap.\n", __PRETTY_FUNCTION__, __LINE__);
    while (in_jq_.empty())
    {
      /** Busy wait until a job is available*/
      sleep_ms(2); // Sleeping to reduce CPU busy time slightly.
    }

    auto &[id, state] = in_jq_.front();
    printf("%s:%d: Got a job [%d]!\n", __PRETTY_FUNCTION__, __LINE__, id);
    state.execute();

    printf("%s:%d: Done [%d]!\n", __PRETTY_FUNCTION__, __LINE__, id);
    out_jq_.emplace_back({id, state});
    in_jq_.pop();
    goto start_run;
  }
#endif

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
    reg offset = 0;

    for (std::size_t i = 0; i < imm.size(); ++i)
      offset[i + offset.size() - imm.size()] = imm[i];
    add_(rd, pc, offset);
    pc = rd;
  };

  void emulator::jal_(reg &rd, const std::bitset<20> imm)
  {

    int offset = 0;
    for (std::size_t i = 0; i < imm.size() - 2; ++i)
      offset += imm[i] << i;

    offset -= imm[imm.size() - 1] << (imm.size() - 1);
    offset <<= 1;
    rd = pc.to_ulong() + (2 << 4);
    pc = pc.to_ulong() + offset;
  };
  void emulator::jalr_(reg &rd, reg &rs1, std::bitset<12> imm)
  {

    int offset = 0;
    for (std::size_t i = 0; i < imm.size() - 2; ++i)
      offset += imm[i] << i;

    offset -= imm[imm.size() - 1] << imm.size() - 1;
    offset <<= 1;

    unsigned int address = rs1.to_ulong() + offset;

    rd = pc.to_ulong() + (1 << 5);
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

  void emulator::lb_(reg &rd, reg &rs1, std::bitset<12> imm)
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

  void emulator::lh_(reg &rd, reg &rs1, std::bitset<12> imm)
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

  void emulator::lw_(reg &rd, reg &rs1, std::bitset<12> imm)
  {
    int offset = 0;
    for (std::size_t i = 0; i < imm.size() - 2; ++i)
      offset += imm[i] << i;

    offset -= imm[imm.size() - 1] << imm.size() - 1;

    const auto addr = rs1.to_ulong() + offset;
    for (std::size_t i = 0; i < xlen; ++i)
      rd[i] = ram[addr + i];
  };

  void emulator::lbu_(reg &rd, reg &rs1, std::bitset<12> imm)
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

  void emulator::lhu_(reg &rd, reg &rs1, std::bitset<12> imm)
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

  void emulator::addi_(reg &rd, reg &rs1, std::bitset<12> imm)
  {
    add_(rd, rs1, sign_extend(imm));
  };

  void emulator::slti_(reg &rd, reg &rs1, std::bitset<12> imm)
  {
    slt_(rd, rs1, sign_extend(imm));
  };

  void emulator::sltiu_(reg &rd, reg &rs1, std::bitset<12> imm)
  {
    rd = rs1.to_ulong() < sign_extend(imm).to_ulong();
  };

  void emulator::xori_(reg &rd, reg &rs1, std::bitset<12> imm)
  {
    rd = rs1 ^ sign_extend(imm);
  };

  void emulator::ori_(reg &rd, reg &rs1, std::bitset<12> imm)
  {
    rd = rs1 | sign_extend(imm);
  };

  void emulator::andi_(reg &rd, reg &rs1, std::bitset<12> imm)
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

  void emulator::sll_(reg &rd, const reg &rs1, const reg &rs2)
  {
    rd = rs1 << rs2.to_ulong();
  }

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

    std::bitset<5> rd_b;
    for (std::size_t i = 0; i < rd_b.size(); ++i)
      rd_b[i] = r[i + 7];

    reg &rd = resolv_rd(rd_b);

    if (opc_r == parse::lui or opc_r == parse::auipc)
    {
      std::bitset<20> imm;
      for (std::size_t i = 0; i < imm.size(); ++i)
        imm[i] = r[i + 12];

      if (opc_r[5])
        lui_(rd, imm);
      else
        auipc_(rd, imm);

      return;
    }
    else if (opc_r == parse::jal)
    {
      /** \todo Too complicated here,
       * just pass a std::bitset<21> directly. */
      std::bitset<20> imm = 0;
      for (std::size_t i = 0; i < 10; ++i)
        imm[i] = r[i + 21];

      imm[10] = r[20];
      for (std::size_t i = 11; i < 19; ++i)
        imm[i] = r[i + 1];
      imm[19] = r[31];

      jal_(rd, imm);
      return;
    }

    std::bitset<5> rs1_b;

    for (std::size_t i = 0; i < 5; ++i)
      rs1_b[i] = r[i + 15];

    auto rs1 = resolv_rd(rs1_b);

    std::bitset<5> rs2_b;
    for (std::size_t i = 0; i < 5; ++i)
      rs2_b[i] = r[i + 15];

    auto rs2 = resolv_rd(rs2_b);

    if (opc_r == parse::jalr)
    {
      std::bitset<12> imm;

      for (std::size_t i = 0; i < 12; ++i)
        imm[i] = r[i + 20];

      jalr_(rd, rs1, imm);
      return;
    }

    if (opc_r == parse::B)
    {

      std::bitset<12> imm = 0;
      for (std::size_t i = 0; i < 4; ++i)
        imm[i] = r[i + 8];

      for (std::size_t i = 4; i < 10; ++i)
        imm[i] = r[21 + i];

      imm[10] = r[7];
      imm[11] = r[31];

      switch (opc_l.to_ulong())
      {
      case 0:
        beq_(rs1, rs2, imm);
        return;
      case 1:
        bne_(rs1, rs2, imm);
        return;
      case 4:
        blt_(rs1, rs2, imm);
        return;
      case 5:
        bge_(rs1, rs2, imm);
        return;
      case 6:
        bltu_(rs1, rs2, imm);
        return;
      case 7:
        bgeu_(rs1, rs2, imm);
        return;
      default:
        // No idea what B is this, continue to next instruction
        /** \todo goto: end*/
        break;
      }
    }

    if (opc_r == parse::I or opc_r == parse::L)
    {
      std::bitset<12> imm;

      for (std::size_t i = 0; i < 12; ++i)
      {
        imm[i] = r[i + 20];
      }

      if (opc_r[4])
      {
        std::bitset<5> shamt = 0;

        for (std::size_t i = 0; i < 5; ++i)
          shamt[i] = r[i + 20];

        switch (opc_r.to_ulong())
        {
        case 0:
          addi_(rd, rs1, imm);
          break;
        case 2:
          slti_(rd, rs1, imm);
          break;
        case 3:
          sltiu_(rd, rs1, imm);
          break;
        case 4:
          xori_(rd, rs1, imm);
          break;
        case 6:
          ori_(rd, rs1, imm);
          break;
        case 7:
          andi_(rd, rs1, imm);
          break;
        case 1:
          slli_(rd, rs1, shamt);
          break;
        case 5:
          if (r[30])
            srli_(rd, rs1, shamt);
          else
            srai_(rd, rs1, shamt);
          break;
        default:
          // No idea what this is. Just keep going.
          break;
        }
      }
      else
      {
        switch (opc_r.to_ulong())
        {
        case 0:
          lb_(rd, rs1, imm);
          break;
        case 1:
          lh_(rd, rs1, imm);
          break;
        case 2:
          lw_(rd, rs1, imm);
          break;
        case 4:
          lbu_(rd, rs1, imm);
          break;
        case 5:
          lhu_(rd, rs1, imm);
          break;
        default:
          // No idea what this is. Just keep going.
          break;
        }
      }
    }
    else if (opc_r == parse::S)
    {
      std::bitset<12> imm;

      for (std::size_t i = 0; i < 5; ++i)
        imm[i] = r[i + 7];
      for (std::size_t i = 5; i < 12; ++i)
        imm[i] = r[i + 25];

      switch (opc_l.to_ulong())
      {
      case 0:
        sb_(rs1, rs2, imm);
        break;
      case 1:
        sh_(rs1, rs2, imm);
        break;
      case 2:
        sw_(rs1, rs2, imm);
        break;
      }
    }
    else if (opc_r == parse::R)
    {
      switch (opc_l.to_ulong())
      {
      case 0:
        if (!r[30])
          add_(rd, rs1, rs2);
        else
          sub_(rd, rs1, rs2);
        break;
      case 1:
        sll_(rd, rs1, rs2);
        break;
      case 2:
        slt_(rd, rs1, rs2);
        break;
      case 3:
        sltu_(rd, rs1, rs2);
        break;
      case 4:
        xor_(rd, rs1, rs2);
        break;
      case 5:
        if (!r[30])
          srl_(rd, rs1, rs2);
        else
          sra_(rd, rs1, rs2);
        break;
      case 6:
        or_(rd, rs1, rs2);
        break;
      case 7:
        and_(rd, rs1, rs2);
        break;
      default:
        // Still no idea.
        break;
      }
    }

    inc_pc();
  }

  std::string emulator::serialize_ram() const
  {
    std::vector<char> ram_c;
    for (const auto b : ram)
      ram_c.emplace_back(b + 48); // ASCII hack


    std::vector<std::string> splitted;
    for (std::size_t n = 0; n < ram.size(); n += xlen)
    {
      // auto begin = ram_c.begin() + n;

      reg bs(ram_c.data() + n, xlen);

      splitted.emplace_back(parse::base36_encode(bs.to_ulong()));
      splitted[n / xlen].append("|");
    }

    std::string out;

    for (const auto &s : splitted)
      out.append(s);

    return out;
  }

} // namespace emulator

#include "RoL/emulation/emulator.hpp"
#include "RoL/parse.hpp"
#if !RoL_testing
#include "RoL/threaded/shared.hpp"
#endif

#include <ranges>
#include <string>
#include <sstream>
#include <vector>

namespace emulation
{

#if !RoL_testing
  void run()
  {
    printf("%s:%d: No jobs, taking a nap.\n", __PRETTY_FUNCTION__, __LINE__);
    while (in_jq_.empty())
    {
    start_run:
    }

    auto [id, state] = in_jq_.front();
    printf("%s:%d: Got a job [%d] in_jq.size() = '%d'!\n", __PRETTY_FUNCTION__, __LINE__, id, in_jq_.size());
    state.execute();

    out_jq_.emplace_back({id, state});
    in_jq_.pop();
    printf("%s:%d: Done [%d]! in_jq.size() now = '%d'\n", __PRETTY_FUNCTION__, __LINE__, id, in_jq_.size());
    goto start_run;
  }
#endif

  template <std::size_t n>
  static constexpr reg sign_extend(std::bitset<n> imm, std::size_t msb_pos = (n - 1))
  {
    reg tmp = 0;
    for (std::size_t i = 0; i < n; ++i)
      tmp.set(i, imm.test(i));

    // Sign extend
    for (int i = n - 1; i < xlen; ++i)
      tmp.set(i, imm.test(msb_pos));
    return tmp;
  }

  template <std::size_t n>
  static constexpr int to_sint(std::bitset<n> bs)
  {
    int rep = 0;
    for (std::size_t i = 0; i < n - 2; ++i)
      rep += bs.test(i) << i;
    rep -= bs.test(n - 1) << (n - 1);
    return rep;
  }

  reg &emulator::resolv_rd(const std::bitset<5> rd)
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

  void emulator::increment_pc() { add_(pc, pc, xlen); }

  void emulator::lui_(reg &rd, const std::bitset<20> imm)
  {
    rd = (imm.to_ulong() << (xlen - imm.size()));
  };

  void emulator::auipc_(reg &rd, const std::bitset<20> imm)
  {
    rd = 0;
    reg offset = 0;

    for (std::size_t i = 0; i < imm.size(); ++i)
      offset.set(i + offset.size() - imm.size(), imm.test(i));
    add_(rd, pc, offset);
    pc = rd;
  };

  void emulator::jal_(reg &rd, const std::bitset<20> imm)
  {
    add_(rd, pc, xlen);
    add_(pc, pc, sign_extend(imm) << 1);
  };

  void emulator::jalr_(reg &rd, const reg rs1, const std::bitset<12> imm)
  {
    add_(rd, pc, xlen);
    add_(pc, rs1, sign_extend(imm) << 1);
  };

  void emulator::beq_(const reg rs1, const reg rs2, const std::bitset<12> imm)
  {
    if (rs1 != rs2)
      return;
    add_(pc, pc, sign_extend(imm) << 1);
  }
  void emulator::bne_(const reg rs1, const reg rs2, const std::bitset<12> imm)
  {
    if (rs1 == rs2)
      return;
    add_(pc, pc, sign_extend(imm) << 1);
  }

  void emulator::blt_(const reg rs1, const reg rs2, const std::bitset<12> imm)
  {
    if (to_sint(rs1) >= to_sint(rs2))
      return;
    add_(pc, pc, sign_extend(imm) << 1);
  }

  void emulator::bge_(const reg rs1, const reg rs2, const std::bitset<12> imm)
  {
    if (to_sint(rs1) < to_sint(rs2))
      return;
    add_(pc, pc, sign_extend(imm) << 1);
  }
  void emulator::bltu_(const reg rs1, const reg rs2, const std::bitset<12> imm)
  {
    if (rs1.to_ulong() >= rs2.to_ulong())
      return;
    add_(pc, pc, sign_extend(imm) << 1);
  }

  void emulator::bgeu_(const reg rs1, const reg rs2, const std::bitset<12> imm)
  {
    if (rs1.to_ulong() < rs2.to_ulong())
      return;
    add_(pc, pc, sign_extend(imm) << 1);
  }

  void emulator::lb_(reg &rd, const reg rs1, const std::bitset<12> imm)
  {
    int offset = 0;
    for (std::size_t i = 0; i < imm.size() - 2; ++i)
      offset += imm.test(i) << i;

    offset -= imm.test(imm.size() - 1) << (imm.size() - 1);

    const auto addr = rs1.to_ulong() + offset;
    for (std::size_t i = 0; i < xlen / 4; ++i)
      rd.set(i, ram.at(addr + i));

    rd = sign_extend(rd, xlen / 4 - 1);
  };

  void emulator::lh_(reg &rd, const reg rs1, const std::bitset<12> imm)
  {
    int offset = 0;
    for (std::size_t i = 0; i < imm.size() - 2; ++i)
      offset += imm.test(i) << i;

    offset -= imm.test(imm.size() - 1) << (imm.size() - 1);

    const auto addr = rs1.to_ulong() + offset;
    for (std::size_t i = 0; i < xlen / 2; ++i)
      rd.set(i, ram.at(addr + i));

    rd = sign_extend(rd, xlen / 2 - 1);
  };

  void emulator::lw_(reg &rd, const reg rs1, const std::bitset<12> imm)
  {
    int offset = 0;
    for (std::size_t i = 0; i < imm.size() - 2; ++i)
      offset += imm.test(i) << i;

    offset -= imm.test(imm.size() - 1) << (imm.size() - 1);

    const auto addr = rs1.to_ulong() + offset;
    for (std::size_t i = 0; i < xlen; ++i)
      rd.set(i, ram.at(addr + i));
  };

  void emulator::lbu_(reg &rd, const reg rs1, const std::bitset<12> imm)
  {
    rd = 0;

    int offset = 0;
    for (std::size_t i = 0; i < imm.size() - 2; ++i)
      offset += imm.test(i) << i;

    offset -= imm.test(imm.size() - 1) << (imm.size() - 1);

    const auto addr = rs1.to_ulong() + offset;
    for (std::size_t i = 0; i < xlen / 4; ++i)
      rd.set(i, ram.at(addr + i));
  };

  void emulator::lhu_(reg &rd, const reg rs1, const std::bitset<12> imm)
  {
    rd = 0;

    int offset = 0;
    for (std::size_t i = 0; i < imm.size() - 2; ++i)
      offset += imm.test(i) << i;

    offset -= imm.test(imm.size() - 1) << (imm.size() - 1);

    const auto addr = rs1.to_ulong() + offset;
    for (int i = 0; i < xlen / 2; ++i)
      rd.set(i, ram.at(addr + i));
  };

  void emulator::sb_(const reg rs1, const reg rs2, const std::bitset<12> imm)
  {
    int offset = 0;
    for (std::size_t i = 0; i < imm.size() - 2; ++i)
      offset += imm.test(i) << i;

    offset -= imm.test(imm.size() - 1) << (imm.size() - 1);

    const auto addr = rs1.to_ulong() + offset;
    for (int i = 0; i < xlen / 4; ++i)
      ram.at(addr + i) = rs2.test(i);
  };

  void emulator::sh_(const reg rs1, const reg rs2, const std::bitset<12> imm)
  {
    int offset = 0;
    for (std::size_t i = 0; i < imm.size() - 2; ++i)
      offset += imm.test(i) << i;

    offset -= imm.test(imm.size() - 1) << (imm.size() - 1);

    const auto addr = rs1.to_ulong() + offset;
    for (int i = 0; i < xlen / 2; ++i)
      ram.at(addr + i) = rs2.test(i);
  };

  void emulator::sw_(const reg rs1, const reg rs2, const std::bitset<12> imm)
  {
    // RAM not implemented yet !
    // This is will just cause stack corrution/decay lol
    int offset = 0;
    for (std::size_t i = 0; i < imm.size() - 2; ++i)
      offset += imm.test(i) << i;

    offset -= imm.test(imm.size() - 1) << (imm.size() - 1);
    const auto addr = rs1.to_ulong() + offset;
    for (int i = 0; i < xlen; ++i)
      ram.at(addr + i) = rs2.test(i);
  };

  void emulator::addi_(reg &rd, const reg rs1, const std::bitset<12> imm)
  {
    printf("addi called\n");
    add_(rd, rs1, sign_extend(imm));
  };

  void emulator::slti_(reg &rd, const reg rs1, const std::bitset<12> imm)
  {
    slt_(rd, rs1, sign_extend(imm));
  };

  void emulator::sltiu_(reg &rd, const reg rs1, const std::bitset<12> imm)
  {
    rd = rs1.to_ulong() < sign_extend(imm).to_ulong();
  };

  void emulator::xori_(reg &rd, const reg rs1, const std::bitset<12> imm)
  {
    rd = rs1 ^ sign_extend(imm);
  };

  void emulator::ori_(reg &rd, const reg rs1, const std::bitset<12> imm)
  {
    rd = rs1 | sign_extend(imm);
  };

  void emulator::andi_(reg &rd, const reg rs1, const std::bitset<12> imm)
  {
    rd = rs1 & sign_extend(imm);
  };

  void emulator::slli_(reg &rd, const reg rs1, const std::bitset<5> shamt)
  {
    rd = rs1 << shamt.to_ulong();
  };

  void emulator::srli_(reg &rd, const reg rs1, const std::bitset<5> shamt)
  {
    rd = rs1 >> shamt.to_ulong();
  };

  void emulator::srai_(reg &rd, const reg rs1, const std::bitset<5> shamt)
  {
    rd = rs1 >> shamt.to_ulong();
    rd.set(rd.size() - 1, rs1.test(rs1.size() - 1));
  };

  void emulator::add_(reg &rd, const reg rs1, const reg rs2)
  {
    bool carry = 0;
    for (int i = 0; i < xlen; ++i)
    {
      rd[i] = (rs1.test(i) ^ rs2.test(i)) ^ carry;
      carry = (rs1.test(i) && rs2.test(i)) || (rs1.test(i) && carry) || (rs2.test(i) && carry);
    }
  };

  void emulator::sub_(reg &rd, const reg rs1, const reg rs2)
  {
    // Compute 2's complement, then do an ADD.
    // This is better than duplicating code.
    emulator::add_(rd, ~rs2, 1);
    emulator::add_(rd, rs1, rd);
  };

  void emulator::sll_(reg &rd, const reg rs1, const reg rs2)
  {
    rd = rs1 << rs2.to_ulong();
  }

  void emulator::slt_(reg &rd, const reg rs1, const reg rs2)
  {
    if (rs1.test(rs1.size() - 1) xor rs2.test(rs2.size() - 1))
    {
      rd = rs1.test(rs1.size() - 1) > rs2.test(rs2.size() - 1);
      return;
    }

    if (rs1.test(rs1.size() - 1) and rs2.test(rs2.size() - 1))
    {
      rd = rs1.to_ulong() > rs2.to_ulong();
      return;
    }

    rd = rs1.to_ulong() < rs2.to_ulong();
    return;
  };

  void emulator::sltu_(reg &rd, const reg rs1, const reg rs2)
  {
    rd = rs1.to_ulong() > rs2.to_ulong(); // ? 1 : 0;
  };

  void emulator::xor_(reg &rd, const reg rs1, const reg rs2)
  {
    rd = rs1 ^ rs2;
  };

  void emulator::srl_(reg &rd, const reg rs1, const reg rs2)
  {
    rd = rs1 >> rs2.to_ulong();
  };

  void emulator::sra_(reg &rd, const reg rs1, const reg rs2)
  {
    rd = (rs1 >> rs2.to_ulong());
    rd.set(rd.size() - 1, rs1.test(rs1.size() - 1));
  };

  void emulator::or_(reg &rd, const reg rs1, const reg rs2)
  {
    rd = rs1 | rs2;
  };

  void emulator::and_(reg &rd, const reg rs1, const reg rs2)
  {
    rd = rs1 & rs2;
  };

  void emulator::execute()
  {

    reg r = 0;

    if (static_cast<std::size_t>(pc.to_ulong() + (xlen << 1)) > ram.size())
    {
      printf("%s:%d: PC (%ld) out-of-bounds, resetting...", __PRETTY_FUNCTION__, __LINE__, pc.to_ulong());
      pc.reset();
      return;
    }

    for (int i = 0; i < xlen; ++i)
    {
      r.set(i, ram.at(pc.to_ulong() + xlen - i - 1));
    }

    std::bitset<7> opc_r;

    for (std::size_t i = 0; i < opc_r.size(); ++i)
      opc_r.set(i, r.test(i));

    std::bitset<3> opc_l;

    for (std::size_t i = 0; i < opc_l.size(); ++i)
      opc_l.set(i, r.test(i + 12));

    std::bitset<5> rd_b;
    for (std::size_t i = 0; i < rd_b.size(); ++i)
      rd_b.set(i, r.test(i + 7));

    reg &rd = resolv_rd(rd_b);

    if (opc_r == parse::lui or opc_r == parse::auipc)
    {
      std::bitset<20> imm;
      for (std::size_t i = 0; i < imm.size(); ++i)
        imm.set(i, r.test(i + xlen - imm.size()));

      if (opc_r.test(5))
        lui_(rd, imm);
      else
      {
        auipc_(rd, imm);
        return;
      }
    }
    else if (opc_r == parse::jal)
    {
      /** \todo Too complicated here,
       * just pass a std::bitset<21> directly. */
      std::bitset<20> imm = 0;
      for (std::size_t i = 0; i < 10; ++i)
        imm.set(i, r.test(i + 21));

      imm.set(10, r.test(20));
      for (std::size_t i = 11; i < 19; ++i)
        imm.set(i, r.test(i + 1));
      imm.set(19, r.test(31));

      jal_(rd, imm);
      return;
    }

    std::bitset<5> rs1_b;

    for (std::size_t i = 0; i < rs1_b.size(); ++i)
      rs1_b.set(i, r.test(i + 15));

    auto rs1 = resolv_rd(rs1_b);

    std::bitset<5> rs2_b;
    for (std::size_t i = 0; i < rs2_b.size(); ++i)
      rs2_b.set(i, r.test(i + 15));

    auto rs2 = resolv_rd(rs2_b);

    if (opc_r == parse::jalr)
    {
      std::bitset<12> imm;

      for (std::size_t i = 0; i < imm.size(); ++i)
        imm.set(i, r.test(i + xlen - imm.size()));

      jalr_(rd, rs1, imm);
      return;
    }

    if (opc_r == parse::B)
    {

      std::bitset<12> imm = 0;
      for (std::size_t i = 0; i < 4; ++i)
        imm.set(i, r.test(i + 8));

      for (std::size_t i = 4; i < 10; ++i)
        imm.set(i, r.test(21 + i));

      imm.set(10, r.test(7));
      imm.set(11, r.test(31));

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

      for (std::size_t i = 0; i < imm.size(); ++i)
      {
        imm.set(i, r.test(i + xlen - imm.size() - 1));
      }

      if (opc_r.test(4))
      {
        std::bitset<5> shamt = 0;

        for (std::size_t i = 0; i < shamt.size(); ++i)
          shamt.set(i, r.test(i + xlen - imm.size()));

        switch (opc_l.to_ulong())
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
          if (r.test(30))
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
        switch (opc_l.to_ulong())
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

    if (opc_r == parse::S)
    {
      std::bitset<12> imm;

      for (std::size_t i = 0; i < 5; ++i)
        imm.set(i, r.test(i + 7));
      for (std::size_t i = 5; i < imm.size(); ++i)
        imm.set(i, r.test(i + 25));

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

    if (opc_r == parse::R)
    {
      switch (opc_l.to_ulong())
      {
      case 0:
        if (!r.test(30))
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
        if (!r.test(30))
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
    increment_pc();
  }

  std::string emulator::serialize_ram() const
  {
    std::vector<char> ram_c;
    for (const auto b : ram)
      ram_c.emplace_back(b + 48); // ASCII hack

    std::vector<std::string> splitted;
    for (std::size_t n = 0; n + xlen < ram_c.size(); n += xlen)
    {
      reg bs(ram_c.data() + n, xlen);

      splitted.emplace_back(parse::base36_encode(bs.to_ulong()));
      splitted.back().append("|");
    }

    std::string out;

    for (const auto &s : splitted)
      out.append(s);

    return out;
  }

} // namespace emulator

#pragma once

#include <bitset>
#include <string>
#include <vector>

#include "RoL/parse.hpp"

namespace emulation
{

  constexpr unsigned char xlen = 32;

  void run();

  class emulator final
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

    std::vector<reg> ram;

    reg &resolv_rd(const std::bitset<5> &rd)
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
        printf("%s:%i: Weird RD content, got '%s'", __PRETTY_FUNCTION__, __LINE__, rd.to_string().c_str());
      }
    }

    /**
     * \fn inc_pc
     *
     * \brief Increments PC by `xlen` (32), usually an optimised add with carry.
     * \warning Not prone to overflow. Trying to execute past maximum (2^32) will not increment anymore.
     * That shouldn't happen anyway, but it's important to know.
     */
    void inc_pc()
    {
      bool carry = false;
      for (int i = xlen - 1 - 5; i < 0; --i)
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
        printf("%s:%i: Exhausted PC past maximum ('%s'). Proceeding anyway.", __PRETTY_FUNCTION__, __LINE__, pc.to_string().c_str());
    }

    /**
     * \fn lui_
     *
     * \brief Load Upper Immediate.
     */
    void lui_(reg &rd, const std::bitset<20> &imm)
    {
      rd = (imm.to_ulong() << (xlen - 20));
    };
    /**
     * \fn auipc_
     *
     * \brief Add Upper Immeddiate to PC.
     */
    void auipc_(reg &rd, const std::bitset<20> imm)
    {
      rd = 0;

      for (int i = 0; i < 12; ++i)
        pc[i] = 0;

      bool carry = 0;
      for (int i = 12; i < 32; ++i)
      {
        if (pc.test(i) and imm.test(i))
        {
          pc[i] = 0 + carry;
          carry = 1;
        }
        else if (pc.test(i) xor imm.test(i) and !carry)
        {
          pc[i] = 1;
        }
        else if (pc.test(i) xor imm.test(i) and carry)
        {
          pc[i] = 0;
        }
        else
        {
          pc[i] = carry;
          carry = 0;
        }
      }

      rd = pc;
    };
    /**
     * \fn jal_
     *
     * \brief Jump And Link.
     */
    void jal_();
    /**
     * \fn jalr_
     *
     * \brief Jump And Link Register.
     */
    void jalr_();
    /**
     * \fn beq_
     *
     * \brief Branch EQual.
     */
    void beq_();
    /**
     * \fn bne_
     *
     * \brief Branch Not Equal.
     */
    void bne_();
    /**
     * \fn blt_
     *
     * \brief Branch Less Than.
     */
    void blt_();
    /**
     * \fn bge_
     *
     * \brief Branch Greater than or Equal.
     */
    void bge_();
    /**
     * \fn bltu_
     *
     * \brief Branch Less Than Unsigned.
     */
    void bltu_();
    /**
     * \fn bgeu_
     *
     * \brief Branch Greater than or Equal Unsigned.
     */
    void bgeu_();
    /**
     * \fn lb_
     *
     * \brief Load Byte.
     */
    void lb_();
    /**
     * \fn lh_
     *
     * \brief Load Halfword.
     */
    void lh_();
    /**
     * \fn lw_
     *
     * \brief Load Word.
     */
    void lw_();
    /**
     * \fn lbu_
     *
     * \brief Load Byte Unsigned.
     */
    void lbu_();
    /**
     * \fn lhu_
     *
     * \brief Load Halfword Unsigned.
     */
    void lhu_();
    /**
     * \fn sb_
     *
     * \brief Store Byte.
     */
    void sb_();
    /**
     * \fn sh_
     *
     * \brief Store Halfword.
     */
    void sh_();
    /**
     * \fn sw_
     *
     * \brief Store Word.
     */
    void sw_();
    /**
     * \fn addi_
     *
     * \brief Add Immediate.
     */
    void addi_();
    /**
     * \fn slti_
     *
     * \brief Set Less Than Immediate.
     */
    void slti_();
    /**
     * \fn sltiu_
     *
     * \brief Set Less Than Immediate Unsigned.
     */
    void sltiu_();
    /**
     * \fn xori_
     *
     * \brief eXclusive OR Immediate.
     */
    void xori_();
    /**
     * \fn ori_
     *
     * \brief OR Immediate.
     */
    void ori_();
    /**
     * \fn andi_
     *
     * \brief AND Immediate.
     */
    void andi_();
    /**
     * \fn slli_
     *
     * \brief Shift Left Logical Immediate.
     */
    void slli_();
    /**
     * \fn stli
     *
     * \brief Shift Right Logical Immediate.
     */
    void srli_();
    /**
     * \fn srai_
     *
     * \brief Shift Right Arithmetic Immediate.
     */
    void srai_();
    /**
     * \fn add_
     *
     * \brief ADD.
     */
    void add_();
    /**
     * \fn sub_
     *
     * \brief SUBstract.
     */
    void sub_();
    /**
     * \fn sll_
     *
     * \brief Shift Left Logical.
     */
    void sll_();
    /**
     * \fn slt_
     *
     * \brief Set Less Than.
     */
    void slt_();
    /**
     * \fn sltu_
     *
     * \brief Set Less Than Unsigned.
     */
    void sltu_();
    /**
     * \fn xor_
     *
     * \brief eXclusive OR.
     */
    void xor_();
    /**
     * \fn srl_
     *
     * \brief Shift Right Logical.
     */
    void srl_();
    /**
     * \fn sra_
     *
     * \brief Shift Right Arithmetic.
     */
    void sra_();
    /**
     * \fn or_
     *
     * \brief OR.
     */
    void or_();
    /**
     * \fn and_
     *
     * \brief AND.
     */
    void and_();

  public:
    void execute()
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
        for (int i = 0; i < imm.size(); ++i)
          imm[i] = r[i + 12];

        std::bitset<5> rd_b;
        for (int i = 0; i < rd_b.size(); ++i)
          rd_b[i] = r[i + 7];

        reg &rd = resolv_rd(rd_b);
        if (opc_r.test(5))
          lui_(rd, imm);
        else
          auipc_(rd, imm);
      }
    }

    /**
     * \fn load_to_ram
     *
     * \brief Loads binary "program" in RAM.
     * \note This is called before execuation.
     */
    void load_to_ram();

    /**
     * \fn serialize_ram
     *
     * \brief Dumps ram content in a serialized format, useful for rendering, for example back to userspace.
     */
    void serialize_ram();
  };

} // namespace emulator

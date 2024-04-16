#pragma once

#include <bitset>
#include <string>
#include <vector>

#include "RoL/parse.hpp"

namespace emulation
{

  constexpr unsigned char xlen = 32;
  using reg = std::bitset<xlen>;

  void run();

  class emulator final
  {
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

    /**
     * \fn resolv_rd
     * \param rd bitset where the rd is encoded as a 5 bit digit.
     * 
     * \brief Returns a reference to the destination register bitset `rd` points to.
    */
    reg &resolv_rd(const std::bitset<5> &);

    /**
     * \fn inc_pc
     *
     * \brief Increments PC by `xlen` (32), using an optimised add with carry.
     * \warning Not prone to overflow. Trying to execute past maximum (2^32) will not increment anymore.
     * That shouldn't happen anyway, but it's important to know.
     */
    void inc_pc();

    /**
     * \fn lui_
     * \param rd
     * \param imm
     * \brief Load Upper Immediate.
     */
    void lui_(reg &, const std::bitset<20> &);

    /**
     * \fn auipc_
     *
     * \brief Add Upper Immeddiate to PC.
     */
    void auipc_(reg &rd, const std::bitset<20> imm);

    /**
     * \fn jal_
     *
     * \brief Jump And Link.
     */
    void jal_(reg& rd, const std::bitset<20> imm);
    /**
     * \fn jalr_
     *
     * \brief Jump And Link Register.
     */
    void jalr_(reg& rd, reg& rs1, std::bitset<12> imm);
    /**
     * \fn beq_
     *
     * \brief Branch EQual.
     */
    void beq_(reg& rs1, reg& rs2, std::bitset<12> imm);
    /**
     * \fn bne_
     *
     * \brief Branch Not Equal.
     */
    void bne_(reg& rs1, reg& rs2, std::bitset<12> imm);
    /**
     * \fn blt_
     *
     * \brief Branch Less Than.
     */
    void blt_(reg& rs1, reg& rs2, std::bitset<12> imm);
    /**
     * \fn bge_
     *
     * \brief Branch Greater than or Equal.
     */
    void bge_(reg& rs1, reg& rs2, std::bitset<12> imm);
    /**
     * \fn bltu_
     *
     * \brief Branch Less Than Unsigned.
     */
    void bltu_(reg& rs1, reg& rs2, std::bitset<12> imm);
    /**
     * \fn bgeu_
     *
     * \brief Branch Greater than or Equal Unsigned.
     */
    void bgeu_(reg& rs1, reg& rs2, std::bitset<12> imm);
    /**
     * \fn lb_
     *
     * \brief Load Byte.
     */
    void lb_(reg& rd, reg& rs1, std::bitset<11> imm);
    /**
     * \fn lh_
     *
     * \brief Load Halfword.
     */
    void lh_(reg& rd, reg& rs1, std::bitset<11> imm);
    /**
     * \fn lw_
     *
     * \brief Load Word.
     */
    void lw_(reg& rd, reg& rs1, std::bitset<11> imm);
    /**
     * \fn lbu_
     *
     * \brief Load Byte Unsigned.
     */
    void lbu_(reg& rd, reg& rs1, std::bitset<11> imm);
    /**
     * \fn lhu_
     *
     * \brief Load Halfword Unsigned.
     */
    void lhu_(reg& rd, reg& rs1, std::bitset<11> imm);
    /**
     * \fn sb_
     *
     * \brief Store Byte.
     */
    void sb_(reg& rs1, reg& rs2, std::bitset<12> imm);
    /**
     * \fn sh_
     *
     * \brief Store Halfword.
     */
    void sh_(reg& rs1, reg& rs2, std::bitset<12> imm);
    /**
     * \fn sw_
     *
     * \brief Store Word.
     */
    void sw_(reg& rs1, reg& rs2, std::bitset<12> imm);
    /**
     * \fn addi_
     *
     * \brief Add Immediate.
     */
    void addi_(reg& rd, reg& rs1, std::bitset<11> imm);
    /**
     * \fn slti_
     *
     * \brief Set Less Than Immediate.
     */
    void slti_(reg& rd, reg& rs1, std::bitset<11> imm);
    /**
     * \fn sltiu_
     *
     * \brief Set Less Than Immediate Unsigned.
     */
    void sltiu_(reg& rd, reg& rs1, std::bitset<11> imm);
    /**
     * \fn xori_
     *
     * \brief eXclusive OR Immediate.
     */
    void xori_(reg& rd, reg& rs1, std::bitset<11> imm);
    /**
     * \fn ori_
     *
     * \brief OR Immediate.
     */
    void ori_(reg& rd, reg& rs1, std::bitset<11> imm);
    /**
     * \fn andi_
     *
     * \brief AND Immediate.
     */
    void andi_(reg& rd, reg& rs1, std::bitset<11> imm);
    /**
     * \fn slli_
     *
     * \brief Shift Left Logical Immediate.
     */
    void slli_(reg& rd, reg& rs1, std::bitset<5> shamt);
    /**
     * \fn stli
     *
     * \brief Shift Right Logical Immediate.
     */
    void srli_(reg& rd, reg& rs1, std::bitset<5> shamt);
    /**
     * \fn srai_
     *
     * \brief Shift Right Arithmetic Immediate.
     */
    void srai_(reg& rd, reg& rs1, std::bitset<5> shamt);
    /**
     * \fn add_
     *
     * \brief ADD.
     */
    void add_(reg& rd, const reg& rs1, const reg& rs2);
    /**
     * \fn sub_
     *
     * \brief SUBstract.
     */
    void sub_(reg& rd, const reg& rs1, const reg& rs2);
    /**
     * \fn sll_
     *
     * \brief Shift Left Logical.
     */
    void sll_(reg& rd, const reg& rs1, const reg& rs2);
    /**
     * \fn slt_
     *
     * \brief Set Less Than.
     */
    void slt_(reg& rd, const reg& rs1, const reg& rs2);
    /**
     * \fn sltu_
     *
     * \brief Set Less Than Unsigned.
     */
    void sltu_(reg& rd, const reg& rs1, const reg& rs2);
    /**
     * \fn xor_
     *
     * \brief eXclusive OR.
     */
    void xor_(reg& rd, const reg& rs1, const reg& rs2);
    /**
     * \fn srl_
     *
     * \brief Shift Right Logical.
     */
    void srl_(reg& rd, const reg& rs1, const reg& rs2);
    /**
     * \fn sra_
     *
     * \brief Shift Right Arithmetic.
     */
    void sra_(reg& rd, const reg& rs1, const reg& rs2);
    /**
     * \fn or_
     *
     * \brief OR.
     */
    void or_(reg& rd, const reg& rs1, const reg& rs2);
    /**
     * \fn and_
     *
     * \brief AND.
     */
    void and_(reg& rd, const reg& rs1, const reg& rs2);

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

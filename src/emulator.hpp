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

    std::vector<std::bitset<xlen>> ram;

    /**
     * \fn lui_
     *
     * \brief Load Unsigned Immediate.
     */
    void lui_();
    /**
     * \fn auipc_
     *
     * \brief Add Upper Immeddiate to PC.
     */
    void auipc_();
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

#include <iostream>
#include <bitset>

#include "RoL/parse.hpp"

#undef NDEBUG
#include <boost/assert.hpp>

using namespace parse;
/**
 * \file test_parse.cpp
 * \brief Sample test unit for the parsing utilies. Validates parsing output.
 * \note Tests are to be run on the host.
 */
int main()
{
    BOOST_VERIFY(s_to_opc.at("add") == opc::ADD);
    BOOST_VERIFY(s_to_opc.at("sub") == opc::SUB);
    BOOST_VERIFY(s_to_opc.at("xor") == opc::XOR);
    BOOST_VERIFY(s_to_opc.at("or") == opc::OR);
    BOOST_VERIFY(s_to_opc.at("and") == opc::AND);
    BOOST_VERIFY(s_to_opc.at("sll") == opc::SLL);
    BOOST_VERIFY(s_to_opc.at("srl") == opc::SRL);
    BOOST_VERIFY(s_to_opc.at("sra") == opc::SRA);
    BOOST_VERIFY(s_to_opc.at("slt") == opc::SLT);
    BOOST_VERIFY(s_to_opc.at("sltu") == opc::SLTU);
    BOOST_VERIFY(s_to_opc.at("addi") == opc::ADDI);
    BOOST_VERIFY(s_to_opc.at("xori") == opc::XORI);
    BOOST_VERIFY(s_to_opc.at("ori") == opc::ORI);
    BOOST_VERIFY(s_to_opc.at("andi") == opc::ANDI);
    BOOST_VERIFY(s_to_opc.at("slli") == opc::SLLI);
    BOOST_VERIFY(s_to_opc.at("srli") == opc::SRLI);
    BOOST_VERIFY(s_to_opc.at("srai") == opc::SRAI);
    BOOST_VERIFY(s_to_opc.at("slti") == opc::SLTI);
    BOOST_VERIFY(s_to_opc.at("sltui") == opc::SLTUI);
    BOOST_VERIFY(s_to_opc.at("sb") == opc::SB);
    BOOST_VERIFY(s_to_opc.at("sh") == opc::SH);
    BOOST_VERIFY(s_to_opc.at("sw") == opc::SW);
    BOOST_VERIFY(s_to_opc.at("beq") == opc::BEQ);
    BOOST_VERIFY(s_to_opc.at("bne") == opc::BNE);
    BOOST_VERIFY(s_to_opc.at("blt") == opc::BLT);
    BOOST_VERIFY(s_to_opc.at("bge") == opc::BGE);
    BOOST_VERIFY(s_to_opc.at("bltu") == opc::BLTU);
    BOOST_VERIFY(s_to_opc.at("bgeu") == opc::BGEU);
    BOOST_VERIFY(s_to_opc.at("lb") == opc::LB);
    BOOST_VERIFY(s_to_opc.at("lh") == opc::LH);
    BOOST_VERIFY(s_to_opc.at("lw") == opc::LW);
    BOOST_VERIFY(s_to_opc.at("lbu") == opc::LBU);
    BOOST_VERIFY(s_to_opc.at("lhu") == opc::LHU);
    BOOST_VERIFY(s_to_opc.at("jal") == opc::JAL);
    BOOST_VERIFY(s_to_opc.at("jalr") == opc::JALR);
    BOOST_VERIFY(s_to_opc.at("lui") == opc::LUI);
    BOOST_VERIFY(s_to_opc.at("auipc") == opc::AUIPC);
    BOOST_VERIFY(s_to_opc.at("ecall") == opc::ECALL);
    BOOST_VERIFY(s_to_opc.at("ebreak") == opc::EBREAK);
}

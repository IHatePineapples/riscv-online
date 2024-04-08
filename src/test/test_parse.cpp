#include <iostream>
#include <string>

#include "RoL/parse.hpp"

#undef NDEBUG
#include <boost/assert.hpp>

using namespace parse;
/***
 * \file test_parse.cpp
 * \brief Sample test unit for the parsing utilies. Validates parsing output.
 * \note Tests are to be run on the host.
 */
int main()
{
    auto i = make_I("addi   x1, x2, 5");

    BOOST_VERIFY(i.has_value());
    const auto [op, oa0, oa1, imm] = *i;
    std::cerr << std::to_string(static_cast<int>(op)) << '\t' << std::to_string(static_cast<int>(oa0)) << ", " << std::to_string(static_cast<int>(oa1)) << ", " << std::to_string(imm) << '\n';
    BOOST_VERIFY(i == I(opc::ADDI, reg::x1, reg::x2, 5));
}

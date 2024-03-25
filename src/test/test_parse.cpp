#include <parse.hpp>
#include <iostream>
#include <string>

#undef NDEBUG
#include <boost/assert.hpp>

using namespace parse;

int main(){
    auto i = make_I("addi   x1, x2, 5");

    BOOST_VERIFY(i.has_value());
    const auto [op, oa0, oa1, imm] = *i;
    std::cerr << std::to_string(static_cast<int>(op)) << '\t' << std::to_string(static_cast<int>(oa0)) << ", " << std::to_string(static_cast<int>(oa1)) << ", " << std::to_string(imm) << '\n';
    BOOST_VERIFY(i == I(opc::ADDI, reg::x1, reg::x2, 5));



}

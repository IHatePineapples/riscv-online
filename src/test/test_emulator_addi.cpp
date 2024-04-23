#include <iostream>

#include "RoL/emulation/emulator.hpp"

#undef NDEBUG
#include <boost/assert.hpp>

inline unsigned long operator*(emulation::reg bs)
{
    return bs.to_ulong();
}

inline unsigned long operator*(std::bitset<11> bs)
{
    return bs.to_ulong();
}

int main(){

    emulation::emulator emu{};

    emulation::reg l, res;
    std::bitset<11> r;


    l = 55;
    r = 200;
    emu.addi_(res, l, r);

    BOOST_VERIFY(*l + *r == *res);
    std::cout << *l << " + " << *r << " = " << *res << std::endl;

    l = 5234;
    r = 323;
    emu.addi_(res, l, r);

    BOOST_VERIFY(*l + *r == *res);
    std::cout << *l << " + " << *r << " = " << *res << std::endl;
    l = 531235;
    r = 222;
    emu.addi_(res, l, r);

    BOOST_VERIFY(*l + *r == *res);
    std::cout << *l << " + " << *r << " = " << *res << std::endl;
    l = 552312;
    r = 255;
    emu.addi_(res, l, r);

    BOOST_VERIFY(*l + *r == *res);
    std::cout << *l << " + " << *r << " = " << *res << std::endl;
    l = 5512121;
    r = 1009;
    emu.addi_(res, l, r);

    BOOST_VERIFY(*l + *r == *res);
    std::cout << *l << " + " << *r << " = " << *res << std::endl;
    l = 514141415;
    r = 1023;
    emu.addi_(res, l, r);

    BOOST_VERIFY(*l + *r == *res);
    std::cout << *l << " + " << *r << " = " << *res << std::endl;
}

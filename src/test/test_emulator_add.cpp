#include <iostream>

#include "RoL/emulation/emulator.hpp"

#undef NDEBUG
#include <boost/assert.hpp>

inline unsigned long operator*(emulation::reg bs)
{
    return bs.to_ulong();
}
int main()
{
    emulation::emulator emu{};

    emulation::reg l, r, res;

    l = 55;
    r = 200;
    emu.add_(res, l, r);

    BOOST_VERIFY(*l + *r == *res);
    std::cout << *l << " + " << *r << " = " << *res << std::endl;

    l = 5234;
    r = 32323;
    emu.add_(res, l, r);

    BOOST_VERIFY(*l + *r == *res);
    std::cout << *l << " + " << *r << " = " << *res << std::endl;
    l = 531235;
    r = 23123300;
    emu.add_(res, l, r);

    BOOST_VERIFY(*l + *r == *res);
    std::cout << *l << " + " << *r << " = " << *res << std::endl;
    l = 552312;
    r = 255500;
    emu.add_(res, l, r);

    BOOST_VERIFY(*l + *r == *res);
    std::cout << *l << " + " << *r << " = " << *res << std::endl;
    l = 5512121;
    r = 2099990;
    emu.add_(res, l, r);

    BOOST_VERIFY(*l + *r == *res);
    std::cout << *l << " + " << *r << " = " << *res << std::endl;
    l = 514141415;
    r = 202323230;
    emu.add_(res, l, r);

    BOOST_VERIFY(*l + *r == *res);
    std::cout << *l << " + " << *r << " = " << *res << std::endl;
}

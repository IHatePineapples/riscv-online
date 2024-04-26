#include <iostream>

#include "RoL/emulation/emulator.hpp"

#undef NDEBUG
#include <boost/assert.hpp>

inline long operator*(const emulation::reg bs)
{
    
    int32_t representation = 0;
    for (std::size_t i = 0; i < bs.size() - 2; ++i)
      representation += bs[i] << i;

    representation -= bs[bs.size() - 1] << bs.size() - 2;
    std::cout << representation << std::endl;
    return representation;
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
    l = -202,323,230;
    r = 514141415;
    emu.add_(res, l, r);

    BOOST_VERIFY(*l + *r == *res);
    std::cout << *l << " + " << *r << " = " << *res << std::endl;
}

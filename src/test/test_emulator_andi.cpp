#include <iostream>

#include "RoL/emulation/emulator.hpp"

#undef NDEBUG
#include <boost/assert.hpp>

int main()
{
    emulation::emulator emu{};

    /** \bug Sign-extension omits last bit. This test catches it.*/

    return 0;

    for (std::size_t n; n < 50000; ++n)
    {
        int32_t bin_l = (rand() - RAND_MAX) * 2; /** Vague attempt to get a signed int */
        int32_t bin_r = rand() >> (emulation::xlen - 12);
        int32_t bin_lr = bin_l & bin_r;
        emulation::reg l = bin_l;
        std::bitset<12> r = bin_r;
        emulation::reg lr;

        emu.andi_(lr, l, r);

        std::cout << l << '\n';
        std::cout << '&' << '\n';
        std::cout << "+++++++++++++++++++++" << r << '\n';
        std::cout << '=' << '\n';
        std::cout << lr << '\n';
        std::cout << std::endl;
        BOOST_VERIFY(lr == bin_lr);
    }
}

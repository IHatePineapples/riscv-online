#include <iostream>

#include "RoL/emulation/emulator.hpp"

#undef NDEBUG
#include <boost/assert.hpp>

int main()
{

    emulation::emulator emu{};

    emulation::reg l, r, res;

    for (std::size_t i = 0; i < (1 << emulation::xlen - 1); ++i)
    {

        l = i;
        for (std::size_t j = 0; j < (1 << emulation::xlen - 1); ++j)
        {
            r = j;

            emu.and_(res, l, r);

            BOOST_VERIFY((l & r) == res);
        }
    }
}

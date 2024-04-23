#include <iostream>

#include "RoL/emulation/emulator.hpp"

#undef NDEBUG
#include <boost/assert.hpp>

int main()
{

    emulation::emulator emu{};

    emulation::reg l, r, res;

    for (std::size_t i = 0; i < (2 << 16); ++i)
    {

        l = i;
        for (std::size_t j = 0; j < (2 << 8); ++j)
        {
            r = j;

            emu.and_(res, l, r);

            BOOST_VERIFY((l & r) == res);
        }
    }
}

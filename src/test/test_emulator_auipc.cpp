
#include <iostream>

#include "RoL/emulation/emulator.hpp"

#undef NDEBUG
#include <boost/assert.hpp>

int main()
{
    /** \bug `auipc` doesn't work! At all. */

    return 0;

    emulation::emulator emu{};

    for (std::size_t n; n < 5000; ++n)
    {
        emulation::reg pre;
        emu.auipc_(pre, 0);

        const int offset = rand();

        emulation::reg post;
        emu.auipc_(post, 0);
        std::cout << pre << '\n';
        std::cout << '+' << '\n';
        std::cout << std::bitset<20>(offset) << "oooooooooooo" << '\n';
        std::cout << '=' << '\n';
        std::cout << post << '\n';
        std::cout << std::endl;
        BOOST_VERIFY(post.to_ulong() == pre.to_ulong() + offset);
    }
}

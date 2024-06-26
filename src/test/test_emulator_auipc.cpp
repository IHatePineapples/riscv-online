
#include <iostream>

#include "RoL/emulation/emulator.hpp"

#undef NDEBUG
#include <boost/assert.hpp>

int main()
{

    emulation::emulator emu{};

    for (std::size_t n; n < 2000; ++n)
    {
        emulation::reg pre;
        emu.auipc_(pre, 0);

        const int offset = (rand() - RAND_MAX / 2) % (2 << 7); // Let's be reasonable in the jump size.

        emulation::reg post;
        emu.auipc_(post, offset);
        std::cout << pre << '\n';
        std::cout << '+' << '\n';
        std::cout << std::bitset<20>(offset) << "oooooooooooo" << ' ' << '(' << offset << ')' << '\n';
        std::cout << '=' << '\n';
        std::cout << post << '\n';
        std::cout << std::endl;
        BOOST_VERIFY(post.to_ulong() == pre.to_ulong() + (offset << 12));
    }
}

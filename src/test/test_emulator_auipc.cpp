
#include <iostream>

#include "RoL/emulation/emulator.hpp"

#undef NDEBUG
#include <boost/assert.hpp>

int main()
{
    /** \warning auipc() doesn't work with negative values! */

    emulation::emulator emu{};

    for (std::size_t n; n < 500; ++n)
    {
        emulation::reg pre;
        emu.auipc_(pre, 0);

        const int offset = rand() % (2<<7); // Let's be reasonable in the jump size.

        emulation::reg post;
        emu.auipc_(post, offset);
        std::cout << pre << '\n';
        std::cout << '+' << '\n';
        std::cout << std::bitset<20>(offset) << "oooooooooooo" << ' ' << '(' << offset << ')' << '\n';
        std::cout << '=' << '\n';
        std::cout << post << '\n';
        std::cout << std::endl;
        BOOST_VERIFY(post.to_ulong() == pre.to_ulong() + (offset<<12));
    }
}

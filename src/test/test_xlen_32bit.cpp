#include "RoL/emulation/emulator.hpp"
#undef NDEBUG
#include <boost/assert.hpp>

int main(){
    BOOST_VERIFY(emulation::xlen == 32);
}

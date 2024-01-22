#include "program.hpp"

namespace emulator {

class Emulator final{

    Program program;


    // Registers member TODO    
    public:

    constexpr Emulator() = default;

    void load_program();


    void parse();
    void step();
    void run();

};


}

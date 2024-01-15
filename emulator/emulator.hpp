#include "program.hpp"
class Emulator final{

    Program program;


    // Registers member TODO    
    public:

    constexpr Emulator();

    void load_program();


    void run();
    void parse();
    void step();

};

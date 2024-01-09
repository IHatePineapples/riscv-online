
class Emulator final{


    // Registers member TODO    
    public:

    constexpr Emulator();

    void load_program();


    void run() const;
    void parse();
    void step() const;

};

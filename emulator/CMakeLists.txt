add_library(Emulator 
    emulator.cpp
    emulator.hpp
    )

target_link_libraries(Emulator PRIVATE Program)

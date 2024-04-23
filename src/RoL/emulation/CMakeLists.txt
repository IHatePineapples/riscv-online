add_library(emulator
    emulator.cpp
    emulator.hpp
)

target_include_directories(emulator 
    PRIVATE
    ../../
)

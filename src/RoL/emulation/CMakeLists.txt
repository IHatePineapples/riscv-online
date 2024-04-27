add_library(emulator
    emulator.cpp
    emulator.hpp
)

target_link_libraries(emulator
    pico_multicore
)
target_include_directories(emulator 
    PRIVATE
    ../../
)

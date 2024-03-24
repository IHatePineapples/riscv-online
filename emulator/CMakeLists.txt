enable_testing()

add_library(Emulator 
    emulator.cpp
    emulator.hpp
    parse.hpp
    )

if( NOT CMAKE_CROSSCOMPILING)
add_subdirectory(test)
endif()

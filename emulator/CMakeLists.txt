enable_testing()

add_library(Emulator 
    emulator.cpp
    emulator.hpp
    parse.hpp
    )


set_target_properties(Emulator PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -pipe -Os -Wextra -fdiagnostics-color=always")

if( NOT CMAKE_CROSSCOMPILING)
    add_subdirectory(test)
endif()

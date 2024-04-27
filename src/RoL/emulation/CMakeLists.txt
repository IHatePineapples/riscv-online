add_library(emulator
    emulator.cpp
    emulator.hpp
)

if(${CMAKE_CROSSCOMPILING})
    
    target_link_libraries(emulator
        pico_multicore
    )

endif()
target_include_directories(emulator 
    PRIVATE
    ../../
)

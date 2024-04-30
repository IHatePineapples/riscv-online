add_library(emulator
    emulator.cpp
    emulator.hpp
)

set_target_properties(emulator PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -pipe -Os -fdiagnostics-color=always -Wall -Wextra")
target_compile_features(emulator PRIVATE cxx_std_23)

if(${CMAKE_CROSSCOMPILING})
    
    target_link_libraries(emulator
        pico_multicore
    )

endif()
target_include_directories(emulator 
    PRIVATE
    ../../
)

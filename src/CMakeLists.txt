add_executable("RISCV_Online" riscv_online.cpp # main() entry-point
    emulator.cpp
    emulator.hpp
    parse.hpp
    server.hpp
    server.cpp
)

set_target_properties("RISCV_Online" PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -pipe -Os -fdiagnostics-color=always")
target_compile_features("RISCV_Online" PRIVATE cxx_std_20)

target_include_directories("RISCV_Online" PRIVATE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries("RISCV_Online" 
    pico_cyw43_arch_lwip_poll 
    pico_stdlib
)

pico_add_extra_outputs("RISCV_Online")


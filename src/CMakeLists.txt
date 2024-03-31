
find_package(Perl)

set(MAKE_FS_DATA_SCRIPT ${CMAKE_SOURCE_DIR}/extern/pico-sdk/lib/lwip/src/apps/http/makefsdata/makefsdata)

add_custom_target(fsdata
    COMMAND perl ${MAKE_FS_DATA_SCRIPT}
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    BYPRODUCTS fsdata.c
)

set(riscv "RISCV_Online")
add_executable(${riscv} riscv_online.cpp # main() entry-point
    emulator.cpp
    emulator.hpp
    parse.hpp
    server.hpp
    server.cpp
    error.hpp
    config.hpp

)
add_dependencies(${riscv} fsdata)
set_target_properties(${riscv} PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -pipe -Os -fdiagnostics-color=always -Wall -Wextra")
target_compile_features(${riscv} PRIVATE cxx_std_20)

target_include_directories(${riscv} PRIVATE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(${riscv} 
    pico_cyw43_arch_lwip_poll 
    pico_lwip_http
    pico_stdlib
)

pico_enable_stdio_usb(${riscv} 1)
pico_enable_stdio_uart(${riscv} 0)

pico_add_extra_outputs(${riscv})

unset(riscv)


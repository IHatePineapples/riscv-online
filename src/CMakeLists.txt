add_subdirectory(RoL/emulation)

find_package(Perl)

set(MAKEFSDATA_SCRIPT ${CMAKE_SOURCE_DIR}/extern/pico-sdk/lib/lwip/src/apps/http/makefsdata/makefsdata)

add_custom_target(fsdata
    COMMAND perl ${MAKEFSDATA_SCRIPT}
    COMMAND mv fsdata.c riscv_fsdata.c
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    BYPRODUCTS riscv_fsdata.c
)

set(riscv "RISCV_Online")
add_executable(${riscv} riscv_online.cpp # main() entry-point
    config.hpp
    post.cpp
    server.cpp
    server.hpp
    ssi.cpp
    ssi.h

)
add_dependencies(${riscv} fsdata)
set_target_properties(${riscv} PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -pipe -Os -fdiagnostics-color=always -Wall -Wextra")
target_compile_features(${riscv} PRIVATE cxx_std_23)

target_include_directories(${riscv} PRIVATE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(${riscv} 
    emulator
    pico_cyw43_arch_lwip_poll 
    pico_lwip_http
    pico_multicore
    pico_stdlib
    pico_sync
)

pico_enable_stdio_usb(${riscv} 1)
pico_enable_stdio_uart(${riscv} 0)

pico_add_extra_outputs(${riscv})

unset(riscv)

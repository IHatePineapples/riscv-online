cmake_minimum_required(VERSION 3.15...3.28)

set(PICO_PLATFORM rp2040)
set(PICO_BOARD pico_w)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 23)

include(extern/pico-sdk/pico_sdk_init.cmake)

project("RISC-V Online"
        VERSION 1.0.1
        LANGUAGES C CXX ASM)

pico_sdk_init()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# Third-party tools and dependencies.
add_subdirectory(extern)


add_subdirectory(src)

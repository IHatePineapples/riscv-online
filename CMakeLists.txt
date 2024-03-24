cmake_minimum_required(VERSION 3.15...3.28)

# Surprise! pico-sdk already does this !
# set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/CMake/Toolchain.cmake)

include(extern/pico-sdk/pico_sdk_init.cmake)

project(
  "RISC-V Online"
  VERSION 0.0.1
  LANGUAGES C CXX ASM)

pico_sdk_init()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
    set(CMAKE_CUDA_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif()


# Third-party tools and dependencies.
add_subdirectory(extern)

add_subdirectory(emulator)
add_subdirectory(app)
add_subdirectory(http)


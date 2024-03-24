cmake_minimum_required(VERSION 3.15...3.28)


set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/CMake/Toolchain.cmake)

include(extern/pico-sdk/pico_sdk_init.cmake)

project(
  "RISC-V Online"
  VERSION 0.0.1
  LANGUAGES C CXX)

enable_language(C)
pico_sdk_init()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe -Os -Wall -Wextra -fdiagnostics-color=always")

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
    set(CMAKE_CUDA_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif()

enable_testing()

# Third-party tools and dependencies.
add_subdirectory(extern)

add_subdirectory(emulator)
add_subdirectory(app)
add_subdirectory(http)


cmake_minimum_required(VERSION 3.9...3.28)

project(
  "RISC-V Online"
  VERSION 0.0.1
  LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
    set(CMAKE_CUDA_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif()

add_subdirectory(extern/wt)

add_subdirectory(parser)
add_subdirectory(emulator)
add_subdirectory(program)
add_subdirectory(app)


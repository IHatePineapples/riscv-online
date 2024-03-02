cmake_minimum_required(VERSION 3.9...3.28)

project(
  "RISC-V Online"
  VERSION 0.0.1
  LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os -Wall -fdiagnostics-color=always")

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
    set(CMAKE_CUDA_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif()

# Disable unused modules from Wt
set( BUILD_EXAMPLES     off)
set( ENABLE_MYSQL       off)
set( ENABLE_MSSQLSERVER off)
set( ENABLE_LIBWTTEST   off)
set( ENABLE_LIBWTDBO    off)

enable_testing()

# Third-party tools and dependencies.
add_subdirectory(extern)

add_subdirectory(emulator)
add_subdirectory(app)


cmake_minimum_required(VERSION 3.13...3.28)

option(PICO_BUILD_DEPLOY "Include building a ufw2 image for the Raspberry Pico W " OFF)

if (PICO_BUILD_DEPLOY)
  include(${CMAKE_SOURCE_DIR}/extern/pico-sdk/pico_sdk_init.cmake)
endif()

project(
  "RISC-V Online"
  VERSION 0.0.1
  LANGUAGES CXX)

if (PICO_BUILD_DEPLOY)
  set(PICO_BOARD pico_w)

  set(CMAKE_SYSTEM_NAME Generic)
  set(CMAKE_SYSTEM_PROCESSOR arm)
  set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

  # set(TARGET_TRIPLET "arm-none-eabi-")
  # set(CMAKE_C_COMPILER   ${TARGET_TRIPLET}gcc)
  # set(CMAKE_CXX_COMPILER ${TARGET_TRIPLET}g++)
  # set(CMAKE_ASM_COMPILER ${TARGET_TRIPLET}gcc)
  # set(CMAKE_LINKER       ${TARGET_TRIPLET}gcc)
  # set(CMAKE_SIZE_UTIL    ${TARGET_TRIPLET}size)
  # set(CMAKE_OBJCOPY      ${TARGET_TRIPLET}objcopy)
  # set(CMAKE_OBJDUMP      ${TARGET_TRIPLET}objdump)
  # set(CMAKE_NM_UTIL      ${TARGET_TRIPLET}gcc-nm)
  # set(CMAKE_AR           ${TARGET_TRIPLET}gcc-ar)
  # set(CMAKE_RANLIB       ${TARGET_TRIPLET}gcc-ranlib)

  pico_sdk_init()
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os -Wall -Wextra -fdiagnostics-color=always")

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


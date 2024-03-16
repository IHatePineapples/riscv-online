cmake_minimum_required(VERSION 3.13...3.28)

option(DEPLOY_PICO "Include building a ufw2 image for the Raspberry Pico W " OFF)


if (DEPLOY_PICO)
  include(CMake/Toolchain.cmake)
  include(extern/pico-sdk/pico_sdk_init.cmake)
endif()

project(
  "RISC-V Online"
  VERSION 0.0.1
  LANGUAGES CXX)

if (DEPLOY_PICO)
  pico_sdk_init()
  set( ENABLE_OPENGL       OFF )
  set( ENABLE_SSL          OFF )
  set( ENABLE_POSTGRES     OFF )
  set( ENABLE_HARU         OFF )
  set( ENABLE_PANGO        OFF )
  set( ENABLE_SQLITE       OFF )
  set( ENABLE_POSTGRES     OFF )
  set( ENABLE_FIREBIRD     OFF )
  set( ENABLE_MYSQL        OFF )
  set( ENABLE_MSSQLSERVER  OFF )
  set( ENABLE_QT4          OFF )
  set( ENABLE_QT5          OFF )
  set( ENABLE_QT6          OFF )
  set( ENABLE_SAML         OFF )
  set( ENABLE_LIBWTTEST    OFF )
  set( ENABLE_LIBWTDBO     OFF )
  set( ENABLE_OPENGL       OFF )
  set( ENABLE_UNWIND       OFF )

endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe -Os -Wall -Wextra -fdiagnostics-color=always")

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


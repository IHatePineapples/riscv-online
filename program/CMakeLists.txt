add_library(Program
    program.cpp
    program.hpp
    )

target_include_directories(Program INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

cmake_minimum_required(VERSION 3.15)

project(RISCV_Online_tests
    LANGUAGES CXX)

add_subdirectory(../RoL/emulation/ ./RoL_emulation_bin.d)

enable_testing()

file(GLOB ALL_TESTS "test_*.cpp")

foreach(single_test_cpp ${ALL_TESTS})
    get_filename_component(single_test_name ${single_test_cpp} NAME_WE)
    
    add_executable(${single_test_name} ${single_test_cpp})
    
    target_compile_features(${single_test_name} PRIVATE cxx_std_20)

    target_link_libraries(${single_test_name} emulator)
    
    target_include_directories(${single_test_name} PRIVATE ../)
    
    add_test(NAME ${single_test_name} COMMAND ${single_test_name})
endforeach()


enable_testing()

file(GLOB ALL_TESTS "*.cpp")

foreach(single_test_cpp ${ALL_TESTS})
    get_filename_component(single_test_name ${single_test_cpp} NAME_WE)
    
    add_executable(${single_test_name} ${single_test_cpp})
    
    target_compile_features(${single_test_name} PRIVATE cxx_std_20)

    link_libraries(${single_test_name} emulator)
    
    target_include_directories(${single_test_name} PRIVATE ../)
    
    add_test(NAME ${single_test_name} COMMAND ${single_test_name})
endforeach()


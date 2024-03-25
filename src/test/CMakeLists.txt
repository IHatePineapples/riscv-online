enable_testing()

add_executable(test_parse
   test_parse.cpp
)

target_compile_features(test_parse PRIVATE cxx_std_20)

link_libraries(test_parse
    emulator
)

target_include_directories(test_parse 
    PRIVATE
    ../)

add_test(NAME test_parse
    COMMAND test_parse)

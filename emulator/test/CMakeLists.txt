add_executable(test_parse
   test_parse.cpp
)

link_libraries(test_parse
    emulator
)

target_include_directories(test_parse 
    PRIVATE
    ../../)

add_test(NAME test_parse
    COMMAND test_parse)

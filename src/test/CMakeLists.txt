enable_testing()

add_executable(test_parse
   test_parse.cpp
)

set_target_properties(test_parse PROPERTIES CMAKE_CXX_STANDARD 17)

link_libraries(test_parse
    emulator
)

target_include_directories(test_parse 
    PRIVATE
    ../)

add_test(NAME test_parse
    COMMAND test_parse)

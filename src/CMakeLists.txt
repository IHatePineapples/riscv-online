add_executable( app
    app.cpp
    emulator.cpp
    emulator.hpp
    parse.hpp
    server.hpp
    server.cpp
)

set_target_properties(app PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -pipe -Os -fdiagnostics-color=always")
target_compile_features(app PRIVATE cxx_std_20)

target_include_directories(app PRIVATE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(app 
    pico_cyw43_arch_lwip_poll 
    pico_stdlib
)

pico_add_extra_outputs(app)


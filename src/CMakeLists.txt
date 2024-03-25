add_executable( app
    app.cpp
    emulator.cpp
    emulator.hpp
    parse.hpp
    server.hpp
    server.cpp
)

set_target_properties(app PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -pipe -Os -fdiagnostics-color=always")
set_target_properties(app PROPERTIES CMAKE_CXX_STANDARD 17)

target_link_libraries(app
    pico_stdlib
    pico_cyw43_arch_lwip_poll
)

pico_add_extra_outputs(app)

add_subdirectory(test)

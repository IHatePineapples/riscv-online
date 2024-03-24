add_executable( app
    app.cpp
)

set_target_properties(app PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -pipe -Os -fdiagnostics-color=always")
set_target_properties(app PROPERTIES CMAKE_CXX_STANDARD 17)

target_include_directories( app PRIVATE 
    ${CMAKE_SOURCE_DIR}/emulator
    )

target_link_libraries(app
    Emulator
    pico_stdlib
)

pico_add_extra_outputs(app)

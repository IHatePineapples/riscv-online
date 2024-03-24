add_executable( app
    app.cpp
)

target_include_directories( app PRIVATE 
    ${CMAKE_SOURCE_DIR}/emulator
    )

target_link_libraries(app
    Emulator
    pico_stdlib
)

pico_add_extra_outputs(app)
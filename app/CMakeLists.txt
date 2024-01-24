add_executable( app
    app.cpp
)

target_include_directories( app PRIVATE ${CMAKE_SOURCE_DIR}/emulator)

target_link_libraries(app
    Emulator
    -lwthttp
    -lwt
)

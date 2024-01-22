add_executable( app
    app.cpp
)

target_include_directories( app PRIVATE ../emulator)

target_link_libraries(app

    Emulator
    Program
    -lwthttp
    -lwt
)

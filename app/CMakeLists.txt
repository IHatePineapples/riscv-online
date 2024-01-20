add_executable( app
    app.cpp
    ui.cpp
    ui.hpp
)

target_link_libraries(app
    -lwthttp
    -lwt
)

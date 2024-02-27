add_executable( app
    app.cpp
)

target_include_directories( app PRIVATE 
    ${CMAKE_SOURCE_DIR}/emulator
    ${CMAKE_SOURCE_DIR}/extern/wt/src/
    ${CMAKE_BINARY_DIR}/extern/wt/
    )

target_link_libraries(app
    Emulator
    wthttp
    wt
)

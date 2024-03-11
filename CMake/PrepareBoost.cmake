macro(PrepareBoost)
   execute_process(
        COMMAND cmake --fresh --toolchain ${CMAKE_SOURCE_DIR}/CMake/Toolchain.cmake --install-prefix ${CMAKE_BIN_DIR}/Boost_root.d -B ${CMAKE_BIN_DIR}/extern/boost -S ${CMAKE_SOURCE_DIR}/extern/boost
        COMMAND cmake --build ${CMAKE_BIN_DIR}/extern/boost
        COMMAND cmake --install ${CMAKE_BIN_DIR}/extern/boost
   )
endmacro(PrepareBoost)

macro(SetBoostRoot)
    set(Boost_ROOT ${CMAKE_BIN_DIR}/Boost_root.d/)
endmacro(SetBoostRoot)

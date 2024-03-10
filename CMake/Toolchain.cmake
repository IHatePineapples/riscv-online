set(PICO_BOARD pico_w)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(TARGET_TRIPLET "arm-none-eabi-")

set(CMAKE_C_COMPILER   ${TARGET_TRIPLET}gcc)
set(CMAKE_CXX_COMPILER ${TARGET_TRIPLET}g++)
set(CMAKE_ASM_COMPILER ${TARGET_TRIPLET}gcc)
set(CMAKE_LINKER       ${TARGET_TRIPLET}gcc)
set(CMAKE_SIZE_UTIL    ${TARGET_TRIPLET}size)
set(CMAKE_OBJCOPY      ${TARGET_TRIPLET}objcopy)
set(CMAKE_OBJDUMP      ${TARGET_TRIPLET}objdump)
set(CMAKE_NM_UTIL      ${TARGET_TRIPLET}gcc-nm)
set(CMAKE_AR           ${TARGET_TRIPLET}gcc-ar)
set(CMAKE_RANLIB       ${TARGET_TRIPLET}gcc-ranlib)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM     NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY     ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE     ONLY)

# Optionally reduce compiler sanity check when cross-compiling.
set(CMAKE_TRY_COMPILE_TARGET_TYPE         STATIC_LIBRARY)
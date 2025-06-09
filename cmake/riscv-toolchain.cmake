set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR riscv)

set(RISCV_PREFIX riscv32-unknown-elf)
set(CMAKE_C_COMPILER   ${RISCV_PREFIX}-gcc)
set(CMAKE_CXX_COMPILER ${RISCV_PREFIX}-g++)
set(CMAKE_ASM_COMPILER ${RISCV_PREFIX}-gcc)

set(CMAKE_FIND_ROOT_PATH root)

# Don't run the linker on compiler check
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# adjust the default behavior of the FIND_XXX() commands:
# search programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# search headers and libraries in the target environment
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

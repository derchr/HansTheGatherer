add_library(WueHans STATIC start.S syscall.c)

target_include_directories(WueHans PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(WueHans PRIVATE RISCV_Options)

target_link_options(WueHans PUBLIC
    -T ${CMAKE_CURRENT_SOURCE_DIR}/wuehans.ld
)

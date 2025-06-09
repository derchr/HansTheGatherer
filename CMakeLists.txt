cmake_minimum_required(VERSION 3.24)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/riscv-toolchain.cmake)

project(HansTheGatherer C CXX ASM)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

FetchContent_Declare(
    entt
    URL https://github.com/skypjack/entt/archive/refs/tags/v3.15.0.tar.gz
    OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(entt)

find_package(entt CONFIG REQUIRED)

add_library(RISCV_Options INTERFACE)
target_compile_options(RISCV_Options INTERFACE
  -fno-exceptions
  -fno-unwind-tables
  -fno-rtti
  -fno-pic # PIC?
  -mno-relax
  -march=rv32im
  -mabi=ilp32
  -std=c++20
)

add_subdirectory(lib)
add_subdirectory(wuehans)

add_executable(HansTheGatherer
    src/main.cpp
    src/audio.cpp
    src/assets.cpp
    src/level.cpp
    src/physics.cpp
    src/render.cpp
)

target_link_options(HansTheGatherer PRIVATE
  -nostartfiles
  -Wl,--gc-sections
  -march=rv32im
  -mabi=ilp32
  -lstdc++
  -lc
  -lgcc
  -mcmodel=medany
)

target_link_libraries(HansTheGatherer PRIVATE
    RISCV_Options
    WueHans
    Hall
    EnTT
)

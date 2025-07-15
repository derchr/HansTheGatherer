cmake_minimum_required(VERSION 3.24.0)
enable_language(C CXX ASM)
project("Cool game")

if(NOT DESKTOPBUILD)
  set(FINAL_TARGET game.elf)
else()
  set(FINAL_TARGET game)
endif()


FetchContent_Declare(
    entt
    URL https://github.com/skypjack/entt/archive/refs/tags/v3.15.0.tar.gz
    OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(entt)
find_package(entt CONFIG REQUIRED)


set(CMAKE_CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED ON)

add_executable(${FINAL_TARGET})

add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_SOURCE_DIR}/../cmake/cmake_copy.cmake
)
add_dependencies(${FINAL_TARGET} copy_assets)


if(NOT DESKTOPBUILD)
  target_include_directories(${FINAL_TARGET}
  PRIVATE include
  )

  target_link_libraries(${FINAL_TARGET}
  ${WUEHANS_ROOT}/lib/newlib/libc.a
  ${WUEHANS_ROOT}/lib/newlib/libg.a
  ${WUEHANS_ROOT}/lib/newlib/libgloss.a
  ${WUEHANS_ROOT}/lib/newlib/libm.a
  ${WUEHANS_ROOT}/lib/newlib/libgcc.a
  ${WUEHANS_ROOT}/lib/libstdc++.a
  ${WUEHANS_ROOT}/lib/libsupc++.a
  fatfs
  Halib
  EnTT
  )

  target_sources(${FINAL_TARGET} PUBLIC
  PUBLIC ${WUEHANS_ROOT}/wuehans/start.S
  PUBLIC ${WUEHANS_ROOT}/wuehans/start.cpp
  PUBLIC ${WUEHANS_ROOT}/wuehans/syscall.c
  )

  target_compile_definitions(${FINAL_TARGET} PRIVATE WUEHANS)

else()
  target_link_libraries(${FINAL_TARGET}
  Halib_Desktop
  EnTT
  )

  target_compile_definitions(${FINAL_TARGET} PRIVATE DESKTOP)
endif()

target_include_directories(${FINAL_TARGET}
  PUBLIC SYSTEM ${WUEHANS_ROOT}/wuehans
  PRIVATE include
)

#███████████████████████████████████████████████████████████
#███████████████████████████████████████████████████████████
#█████████████ Below starts your realm █████████████████████
#███████████████████████████████████████████████████████████
#███████████████████████████████████████████████████████████


target_sources(${FINAL_TARGET} PUBLIC
  # Add your .cpp files here
  main.cpp
  audio.cpp
  assets.cpp
  level.cpp
  physics.cpp
  render.cpp
)

# This is stuff for debugging the hardware. Just ignore it :)
# add_custom_command(TARGET ${FINAL_TARGET} POST_BUILD
#	COMMAND objcopy ARGS -O verilog ${FINAL_TARGET} Game.hex)
#
# add_custom_command(TARGET ${FINAL_TARGET} POST_BUILD
#	COMMAND python ARGS ${WUEHANS_ROOT}/wuehans/hex8tohex32.py Game.hex > Game32.hex
#)

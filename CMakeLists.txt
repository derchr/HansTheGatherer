cmake_minimum_required(VERSION 3.24.0)

#If you want to build for desktop, use the flag -DDESKTOPBUILD, when running cmake
option(DESKTOPBUILD "If ON, the result will run on your system, if OFF (default) the result will run on wueHans")

if(NOT DESKTOPBUILD)
	set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/toolchain.cmake)
	set(WUEHANS_LINKERSCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/wuehans/wuehans.ld)
endif()

set(WUEHANS_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED ON)

project("WueHans Template Game")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if(NOT DESKTOPBUILD)
  include_directories(
  SYSTEM ${WUEHANS_ROOT}/system-include/libstdc++
  SYSTEM ${WUEHANS_ROOT}/system-include/libstdc++/riscv32-unknown-elf
  SYSTEM ${WUEHANS_ROOT}/system-include
  SYSTEM ${WUEHANS_ROOT}/lib
  SYSTEM ${WUEHANS_ROOT}/system-include/newlib # This needs to be after libstdc++ because of include_nexts - JH
  )
endif()

include(FetchContent)
FetchContent_Declare(Halib
    GIT_REPOSITORY https://github.com/byter64/Halib.git
    GIT_TAG v3.1
)
FetchContent_MakeAvailable(Halib)

if(NOT DESKTOPBUILD)
	add_subdirectory(lib)
endif()

if(NOT DESKTOPBUILD)
	add_compile_options(
	  -fno-exceptions
	  -nostdlib
	  -fno-unwind-tables
	  -fno-rtti
	  -march=rv32imf
      -mabi=ilp32f
	  -Os
	  #-ffreestanding  #Because we use libstdc++, we need to pretend that we are in a hosted environment so that gcc gives us all features of libstdc++
	  -Bstatic
	  -nostdinc
	  -fno-builtin
	)

	add_link_options(
	  -nostdlib
	  -T ${WUEHANS_LINKERSCRIPT}
	  -Wl,--gc-sections
	  -ffreestanding
	  -Bstatic
	  -nostdlib
	  -fno-builtin
	)
endif()

add_subdirectory(code)

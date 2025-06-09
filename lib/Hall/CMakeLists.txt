add_library(Hall STATIC
  source/Audio.cpp
  source/System.cpp
  source/Video.cpp
)

target_include_directories(Hall
  PUBLIC SYSTEM include
)

target_link_libraries(Hall PRIVATE RISCV_Options)

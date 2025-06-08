cmake_minimum_required(VERSION 3.24)

project(HansTheGatherer)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Option to switch real platform vs. SDL implementation...

include(FetchContent)

FetchContent_Declare(
    entt
    URL https://github.com/skypjack/entt/archive/refs/tags/v3.15.0.tar.gz
    OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(entt)
find_package(entt CONFIG REQUIRED)

add_executable(HansTheGatherer
    src/main.cpp
    src/audio.cpp
    src/assets.cpp
    src/level.cpp
    src/physics.cpp
    src/render.cpp
)

target_link_libraries(HansTheGatherer EnTT)

set_property(TARGET HansTheGatherer PROPERTY CXX_STANDARD 20)

cmake_minimum_required(VERSION 3.24)

project(HansTheGatherer)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Option to switch real platform vs. SDL implementation...

include(FetchContent)
FetchContent_Declare(
    SDL3
    URL https://github.com/libsdl-org/SDL/releases/download/release-3.2.14/SDL3-3.2.14.tar.gz
    OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(SDL3)

FetchContent_Declare(
    SDL3_ttf
    URL https://github.com/libsdl-org/SDL_ttf/releases/download/release-3.2.2/SDL3_ttf-3.2.2.tar.gz
    OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(SDL3_ttf)

FetchContent_Declare(
    flecs
    URL https://github.com/SanderMertens/flecs/archive/refs/tags/v4.0.5.tar.gz
    OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(flecs)

find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_ttf CONFIG REQUIRED)
find_package(flecs CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_executable(HansTheGatherer
    src/main.cpp
    src/audio.cpp
    src/assets.cpp
    src/physics.cpp
)

target_link_libraries(HansTheGatherer SDL3::SDL3 SDL3_ttf::SDL3_ttf flecs::flecs spdlog::spdlog)

set_property(TARGET HansTheGatherer PROPERTY CXX_STANDARD 20)

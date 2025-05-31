cmake_minimum_required(VERSION 3.24)

project(HansTheGatherer)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Option to switch real platform vs. SDL implementation...

find_package(SDL3 CONFIG REQUIRED)
find_package(flecs CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_executable(HansTheGatherer
    src/main.cpp
    src/assets.cpp
    src/physics.cpp
)

target_link_libraries(HansTheGatherer SDL3::SDL3 flecs::flecs spdlog::spdlog)

set_property(TARGET HansTheGatherer PROPERTY CXX_STANDARD 20)

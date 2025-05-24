cmake_minimum_required(VERSION 3.24)

project(HansTheGatherer)

# Option to switch real platform vs. SDL implementation...

find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(flecs CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_executable(HansTheGatherer main.cpp)

target_link_libraries(HansTheGatherer SDL3::SDL3 SDL3_image::SDL3_image flecs::flecs spdlog::spdlog)

set_property(TARGET HansTheGatherer PROPERTY CXX_STANDARD 20)

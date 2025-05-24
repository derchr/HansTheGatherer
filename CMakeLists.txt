cmake_minimum_required(VERSION 3.24)

project(HansTheGatherer)

# Option to switch real platform vs. SDL implementation...

find_package(SDL3 REQUIRED)

add_executable(HansTheGatherer main.cpp)

target_link_libraries(HansTheGatherer SDL3::SDL3)

set_property(TARGET HansTheGatherer PROPERTY CXX_STANDARD 23)

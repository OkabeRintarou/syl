cmake_minimum_required(VERSION 3.22)
project(tinyrender)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(tinyrender main.cpp sdl_helper.cpp context.cpp model.cpp)
target_link_libraries(tinyrender ${SDL2_LIBRARIES})

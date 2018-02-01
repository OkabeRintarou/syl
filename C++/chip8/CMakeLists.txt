cmake_minimum_required(VERSION 3.5)
project(chip8)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

add_executable(chip8
        include/chip8.h
        src/chip8.cc
        src/main.cc)
target_link_libraries(chip8 SDL2)

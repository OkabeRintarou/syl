cmake_minimum_required(VERSION 3.15)
project(RDNA2-Disassembler)

set(CMAKE_CXX_STANDARD 20)

add_executable(RDNA2-Disassembler Stream.cpp Disassembler.cpp Main.cpp)

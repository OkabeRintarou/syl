cmake_minimum_required(VERSION 3.22)
project(ParallelScan LANGUAGES CXX CUDA)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenMP)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")

add_executable(scan_test main.cu timer.cpp util.cpp)
add_executable(perf_test.cpp timer.cpp util.cpp)

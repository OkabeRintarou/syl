cmake_minimum_required(VERSION 3.22)
project(ParallelScan)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenMP)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")

add_executable(ParallelScan main.cpp timer.cpp util.cpp)
target_link_libraries(ParallelScan pthread)
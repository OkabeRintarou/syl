cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)

add_executable(bench-throughput bench-throughput.cpp)
target_link_libraries(bench-throughput boost_program_options pthread)
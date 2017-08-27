project(tetris)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES tetris.cpp main.cpp)
add_executable(tetris ${SOURCE_FILES})
target_link_libraries(tetris SDL2)

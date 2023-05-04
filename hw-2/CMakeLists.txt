cmake_minimum_required(VERSION 3.0.0)
project(hw2 VERSION 0.1.0)

add_subdirectory(lib/pipe_parser)
add_subdirectory(lib/operations)
add_subdirectory(lib/messages)

add_executable(hw2 src/main.cpp)
target_link_libraries(hw2 pipe_parser operations messages)
cmake_minimum_required(VERSION 3.16)
project(stack)
 
include_directories(include)
add_executable(stack src/main.cpp src/stack.cpp)

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}")
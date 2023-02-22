cmake_minimum_required(VERSION 3.16)
project(Queue)
 
include_directories(include)
add_executable(queue src/main.cpp src/queue.cpp)

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}")
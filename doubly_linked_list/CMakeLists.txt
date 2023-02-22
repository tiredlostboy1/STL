cmake_minimum_required(VERSION 3.16)
project(doubly_linked_list)
 
include_directories(include)
add_executable(doubly_linked_list src/main.cpp src/doubly_linked_list.cpp)

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}")
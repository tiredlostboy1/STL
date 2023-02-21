cmake_minimum_required(VERSION 3.16)
project(vector)
 
include_directories(include)
add_executable(vector src/main.cpp src/vector.cpp)

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}")
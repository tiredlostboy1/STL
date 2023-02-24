cmake_minimum_required(VERSION 3.16)
project(unordered_map)
 
include_directories(include)
add_executable(unordered_map src/main.cpp)

add_executable(cpp_test tests/test.cpp)

enable_testing()

add_test(
    NAME cpp_test
    COMMAND $<TARGET_FILE:cpp_test>
)


set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}")
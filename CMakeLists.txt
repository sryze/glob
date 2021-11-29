cmake_minimum_required(VERSION 3.1)

project(glob CXX)

add_library(glob STATIC glob.cpp glob.h)
set_target_properties(glob PROPERTIES CXX_STANDARD 14)

add_executable(glob_test glob_test.cpp)
set_target_properties(glob_test PROPERTIES CXX_STANDARD 14)
target_link_libraries(glob_test glob)

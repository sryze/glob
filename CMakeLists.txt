cmake_minimum_required(VERSION 2.8)

project(glob CXX)

include_directories(include)
add_library(glob STATIC
    include/glob.h
    src/glob.cpp
    src/glob_impl_extended.cpp
    src/glob_impl_native.cpp
)

add_executable(glob_test tests/glob_test.cpp)
target_link_libraries(glob_test glob)

cmake_minimum_required(VERSION 3.30)
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_CXX_FLAGS "-std=c++23 -stdlib=libc++")
project("clyth_compiler")
add_subdirectory(src)



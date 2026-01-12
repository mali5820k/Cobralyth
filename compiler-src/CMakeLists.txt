cmake_minimum_required(VERSION 3.30)
set(CMAKE_CXX_COMPILER zig c++)
set(CMAKE_C_COMPILER zig cc)
set(CMAKE_CXX_FLAGS "-std=c++17")
set(CMAKE_C_LINKER_DEPFILE_SUPPORTED FALSE)
set(CMAKE_CXX_LINKER_DEPFILE_SUPPORTED FALSE)

project(
    clyth_compiler_bin
    VERSION 1.0.0
    LANGUAGES CXX
)

add_executable(clyth_compiler_bin)
add_subdirectory(src)
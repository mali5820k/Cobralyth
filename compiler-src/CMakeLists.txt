cmake_minimum_required(VERSION 3.30)
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_C_COMPILER "/usr/bin/clang")
set(CMAKE_CXX_FLAGS "-std=c++17 -stdlib=libc++")
project(
    clyth_compiler_bin
    VERSION 1.0.0
    LANGUAGES CXX
)

add_executable(clyth_compiler_bin)
add_subdirectory(src)
target_include_directories(clyth_compiler_bin PRIVATE "-I /usr/x86_64-linux-musl/include/")
target_link_options(clyth_compiler_bin PRIVATE "-static -L /usr/x86_64-linux-musl/lib64/")
cmake_minimum_required(VERSION 3.30)
set(CMAKE_CXX_COMPILER zig c++)
set(CMAKE_C_COMPILER zig cc)
set(CMAKE_CXX_FLAGS "-std=c++17 -static")
set(CMAKE_C_FLAGS "-static")
project(
    clyth_compiler_bin
    VERSION 1.0.0
    LANGUAGES CXX
)

add_executable(clyth_compiler_bin)
add_subdirectory(src)
# target_include_directories(clyth_compiler_bin PRIVATE

# )
# target_link_libraries(clyth_compiler_bin PRIVATE
# )
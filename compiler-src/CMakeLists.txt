cmake_minimum_required(VERSION 3.30)

set(CMAKE_CXX_COMPILER zig c++)
set(CMAKE_C_COMPILER zig cc)
set(CMAKE_CXX_FLAGS "-std=c++17")
set(CMAKE_C_LINKER_DEPFILE_SUPPORTED FALSE)
set(CMAKE_CXX_LINKER_DEPFILE_SUPPORTED FALSE)

project(clyth_compiler_bin VERSION 1.0.0 LANGUAGES C CXX)

find_package(LLVM REQUIRED CONFIG)

add_executable(clyth_compiler_bin)
add_subdirectory(fmt)

target_include_directories(clyth_compiler_bin PUBLIC
    ${CMAKE_SOURCE_DIR}/../antlr4-cpp-runtime-lib/usr/local/include/antlr4-runtime/
    ${LLVM_INCLUDE_DIRS}
)

target_compile_definitions(clyth_compiler_bin PUBLIC
    ${LLVM_DEFINITIONS}
)

llvm_map_components_to_libnames(LLVM_LIBS
    Core
    Support
)

target_link_libraries(clyth_compiler_bin PUBLIC
    fmt::fmt
    ${CMAKE_SOURCE_DIR}/../antlr4-cpp-runtime-lib/usr/local/lib64/libantlr4-runtime.a
    ${LLVM_LIBS}
)

add_subdirectory(src)
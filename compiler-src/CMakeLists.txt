cmake_minimum_required(VERSION 3.30)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_LINKER_DEPFILE_SUPPORTED FALSE)
set(CMAKE_CXX_LINKER_DEPFILE_SUPPORTED FALSE)

project(clyth_compiler_bin VERSION 1.0.0 LANGUAGES C CXX)

find_package(LLVM REQUIRED CONFIG)

set(ANTLR_ROOT "${CMAKE_SOURCE_DIR}/../antlr4-cpp-runtime-lib/usr/local")
find_library(ANTLR4_RUNTIME_LIB
    NAMES antlr4-runtime
    PATHS
        "${ANTLR_ROOT}/lib64"
        "${ANTLR_ROOT}/lib"
    NO_DEFAULT_PATH
)
if(NOT ANTLR4_RUNTIME_LIB)
    message(FATAL_ERROR "Could not find static ANTLR4 runtime under ${ANTLR_ROOT}/lib64 or ${ANTLR_ROOT}/lib")
endif()
message(STATUS "Using ANTLR4 runtime: ${ANTLR4_RUNTIME_LIB}")

add_executable(clyth_compiler_bin)

set(FMT_TEST OFF CACHE BOOL "" FORCE)
set(FMT_DOC OFF CACHE BOOL "" FORCE)
set(FMT_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(fmt)

target_include_directories(clyth_compiler_bin PUBLIC
    ${ANTLR_ROOT}/include/antlr4-runtime/
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
    ${ANTLR4_RUNTIME_LIB}
    ${LLVM_LIBS}
)

add_subdirectory(src)

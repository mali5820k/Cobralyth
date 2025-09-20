cmake_minimum_required(VERSION 3.30)
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_C_COMPILER "/usr/bin/clang")
set(CMAKE_CXX_FLAGS "-std=c++17 -static -nostdinc -nostdlib")
set(CMAKE_C_FLAGS "-static -nostdinc -nostdlib")
project(
    clyth_compiler_bin
    VERSION 1.0.0
    LANGUAGES CXX
)

add_executable(clyth_compiler_bin)
add_subdirectory(src)
target_include_directories(clyth_compiler_bin PRIVATE
    ./libs_build/musl_libc_build/include/
    ./libs_build/llvm-libc++_build/include/c++/v1/
    # Add in the clang resource headers after building clang too!
    # ACTUALLY - check this approach out first - apparently this is something to do with -isystem or something:
    # https://stackoverflow.com/questions/79219330/how-can-i-compile-with-only-llvm
)
target_link_libraries(clyth_compiler_bin PRIVATE
    ./libs_build/musl_libc_build/lib/crt1.o
    ./libs_build/musl_libc_build/lib/crti.o
    ./libs_build/musl_libc_build/lib/critn.o
    ./libs_build/llvm-libc++_build/lib/libc++.a
    ./libs_build/llvm-libc++_build/lib/libc++abi.a
    ./libs_build/llvm-libc++_build/lib/libc++experimental.a
    ./libs_build/llvm-libc++_build/lib/libunwind.a
    ./libs_build/musl_libc_build/lib/libc.a
    ./libs_build/musl_libc_build/lib/libcrypt.a
    ./libs_build/musl_libc_build/lib/libdl.a
    ./libs_build/musl_libc_build/lib/libm.a
    ./libs_build/musl_libc_build/lib/libpthread.a
    ./libs_build/musl_libc_build/lib/libresolv.a
    ./libs_build/musl_libc_build/lib/librt.a
    ./libs_build/musl_libc_build/lib/libutil.a
    ./libs_build/musl_libc_build/lib/libxnet.a
    ./libs_build/musl_libc_build/lib/rcrt1.o
    ./libs_build/musl_libc_build/lib/Scrt1.o
)
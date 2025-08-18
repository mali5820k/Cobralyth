cmake_minimum_required(VERSION 3.30)
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_C_COMPILER "/usr/bin/clang")

# Below CXX and C flags were purposefully set to not use any system libs nor other C or C++ std libraries for distribution and licensing preferences
# ie both llvm-libc and libc++ are licensed under Apache Version 2.0 with LLVM Exceptions.
# Also these flags work with older cmake versions as well as a bonus.
set(CMAKE_CXX_FLAGS "-std=c++17 -stdlib=libc++ -static -nostdinc -nostdlib -I libc/include -I $ENV{LLVM_LIBC_PATH}/include $ENV{LLVM_LIBC_PATH}/startup/linux/crt1.o $ENV{LLVM_LIBC_PATH}/lib/libc.a ")
set(CMAKE_C_FLAGS "-static -nostdinc -nostdlib -I libc/include -I $ENV{LLVM_LIBC_PATH}/include $ENV{LLVM_LIBC_PATH}/startup/linux/crt1.o $ENV{LLVM_LIBC_PATH}/lib/libc.a ")
project("clyth_compiler_bin")
add_subdirectory(src)

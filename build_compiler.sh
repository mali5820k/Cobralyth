#!/bin/bash
CUR_DIR=$(dirname $0)
cd $CUR_DIR

# Confirm clang++, cmake, make, and libcxx (libc++) are all installed:
MISSING=0
clang++ --version
CLANG_PLUS_PLUS=$?
if [[ $CLANG_PLUS_PLUS != 0 ]]; then
    printf "Missing clang++ install!\n"
    MISSING=1
fi

cmake --version
CMAKE=$?
if [[ $CMAKE != 0 ]]; then
    printf "Missing cmake install!\n"
    MISSING=1
fi

dnf list --installed libcxx
LIBCPP=$?
if [[ $LIBCPP != 0 ]]; then
    printf "Missing libcxx (libc++) install!\n"
    MISSING=1
fi

make --version
MAKE=$?
if [[ $CLANG_PLUS_PLUS != 0 ]]; then
    printf "Missing make install!\n"
    MISSING=1
fi

# Exit out if packages are missing, since the script is NOT meant to install them for you.
if [[ $MISSING == 1 ]]; then
    printf "Please install LLVM, LLVM-dev, and mentioned missing packages for build to continue.\nExiting...\n"
    exit -1
fi

CMAKE_LISTS_FILE_PATH=../compiler-src/CMakeLists.txt
mkdir -p build/
cd build/

cmake $CMAKE_LISTS_FILE_PATH

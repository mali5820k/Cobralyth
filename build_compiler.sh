#!/bin/bash
CUR_DIR=$(dirname $0)
cd $CUR_DIR
CUR_DIR=$(pwd)

# Confirm cmake, make, and zig are installed:
MISSING=0
zig version
ZIG=$?
if [[ $ZIG != 0 ]]; then
    printf "Missing zig install!\n"
    MISSING=1
fi

cmake --version
CMAKE=$?
if [[ $CMAKE != 0 ]]; then
    printf "Missing cmake install!\n"
    MISSING=1
fi

make --version
MAKE=$?
if [[ $MAKE != 0 ]]; then
    printf "Missing make install!\n"
    MISSING=1
fi

dnf list llvm-dev* --installed
LLVM_DEVEL=$?
if [[ $LLVM_DEVEL != 0 ]]; then
    printf "Missing llvm-devel install!\n"
    MISSING=1
fi

dnf list llvm --installed
LLVM=$?
if [[ $LLVM != 0 ]]; then
    printf "Missing llvm install!\n"
    MISSING=1
fi

# Exit out if packages are missing, since the script is NOT meant to install them for you.
if [[ $MISSING == 1 ]]; then
    printf "Please install llvm, llvm-devel, and mentioned missing packages for build to continue.\nExiting...\n"
    exit -1
fi

# The overarching-goal of the below from-source builds is to 
# provide an out-of-the-box solution that's self-contained for current and future static linkages for Clyth.


CMAKE_LISTS_FILE_PATH=./compiler-backend/
rm -rf build/
cmake -S $CMAKE_LISTS_FILE_PATH -Bbuild -DCMAKE_C_COMPILER="zig cc" -DCMAKE_CXX_COMPILER="zig c++"
cd build/
make

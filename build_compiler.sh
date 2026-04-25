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

dnf list python3-pip --installed
PIP=$?
if [[ $PIP != 0 ]]; then
    printf "Missing pip install!\n"
    MISSING=1
fi

pip show antlr4-tools
ANTLR4_TOOLS=$?
if [[ $ANTLR4_TOOLS != 0 ]]; then
    printf "Missing antlr4-tools install!\n"
    MISSING=1
else
    local VERSION=4.13.2
    printf "Antlr4-tools is installed - running antlr4 to get $VERSION installed...\n"
    antlr4 -v $VERSION
    printf "\nCompleted Antlr4 setup\n"
fi

# Exit out if packages are missing, since the script is NOT meant to install them for you.
if [[ $MISSING == 1 ]]; then
    printf "Please install llvm, llvm-devel, and any mentioned missing packages for build to continue.\nExiting...\n"
    exit -1
fi

# Clone fmt library:
if [[ ! -d $CUR_DIR/compiler-src/fmt ]]; then
    git clone https://github.com/fmtlib/fmt.git $CUR_DIR/compiler-src/fmt
fi

# The overarching-goal of the below from-source builds is to 
# provide an out-of-the-box solution that's self-contained for current and future static linkages for Clyth.

# Adding the current directory to the path so zig-c.sh and zig-c++.sh can be called;
export PATH=$PATH:$CUR_DIR/

# Build the antlr4-cpp-runtime-src project:
if [[ ! -d $CUR_DIR/antlr4-cpp-runtime-lib/usr/local/include/ ]]; then
    rm -rf $CUR_DIR/antlr4-cpp-runtime-lib/
    mkdir -p $CUR_DIR/antlr4-cpp-runtime-lib/
    cd $CUR_DIR/antlr4-cpp-runtime-src/
    rm -rf build/ run/
    mkdir build && mkdir run && cd build
    cmake .. -DCMAKE_C_COMPILER="zig-c.sh" -DCMAKE_CXX_COMPILER="zig-c++.sh"
    make
    DESTDIR=$CUR_DIR/antlr4-cpp-runtime-lib/ make install
    printf "\nFinished building Antlr4 C++ runtime\n"
else
    printf "\nAntlr4 C++ runtime already built\nSkipping rebuild..."
fi
# Bulid the Clyth Compiler:
printf "\nStarting Clyth Compiler build...\n"
CMAKE_LISTS_FILE_PATH=$CUR_DIR/compiler-src/
COMPILER_BUILD_PATH=$CUR_DIR/build-compiler/
rm -rf $COMPILER_BUILD_PATH
cmake -S $CMAKE_LISTS_FILE_PATH -B${COMPILER_BUILD_PATH} -DCMAKE_C_COMPILER="zig-c.sh" -DCMAKE_CXX_COMPILER="zig-c++.sh"
cd $COMPILER_BUILD_PATH
make
printf "\nFinished building Clyth compiler\n"

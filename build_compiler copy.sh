#!/bin/bash
CUR_DIR=$(dirname $0)
cd $CUR_DIR
CUR_DIR=$(pwd)

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

make --version
MAKE=$?
if [[ $MAKE != 0 ]]; then
    printf "Missing make install!\n"
    MISSING=1
fi

podman --version
podman=$?

if [[ $podman != 0 ]]; then
    printf "Missing podman installation!"
    MISSING=1
fi

# Exit out if packages are missing, since the script is NOT meant to install them for you.
if [[ $MISSING == 1 ]]; then
    printf "Please install LLVM, LLVM-dev, and mentioned missing packages for build to continue.\nExiting...\n"
    exit -1
fi

# The overarching-goal of the below from-source builds is to 
# provide an out-of-the-box solution that's self-contained for current and future static linkages for Clyth.
mkdir -p libs_build/
cd libs_build/

if [[ ! -d musl ]]; then 
    # Clone and build musl libc to use as the libc-of-choice for static linkage for clyth's runtime libraries.
    git clone https://git.musl-libc.org/git/musl
fi

cd musl/
rm -rf build/
chmod +x configure

# Configure to build musl to a local directory:
./configure --prefix=../musl_libc_build/ --enable-optimize --disable-shared
make
make install

# Now use the built musl-libc for static compiles via clang:

CMAKE_LISTS_FILE_PATH=./compiler-src/
rm -rf build/
cmake -S $CMAKE_LISTS_FILE_PATH -Bbuild
cd build/
make

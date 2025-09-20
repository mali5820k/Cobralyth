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

CONTAINER_SYSTEM=podman
podman --version
PODMAN=$?

if [[ $PODMAN != 0 ]]; then
    printf "Missing podman installation\nLooking instead for Docker installation to continue..."

    if [[ $DOCKER != 0 ]]; then
        printf "Missing docker installation!\n"
        MISSING=1
    else
        CONTAINER_SYSTEM=docker
    fi
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
    # Clone and build musl libc to use as the libc-of-choice for building LLVM's libc++
    (
        git clone https://git.musl-libc.org/git/musl
    ) &
fi
if [[ ! -d llvm-project ]]; then
    # Clone and build LLVM libc++ in the background while building musl lib
    (
        git clone https://github.com/llvm/llvm-project.git
    ) &
fi
wait $(jobs -p)

$CONTAINER_SYSTEM pull alpine
$CONTAINER_SYSTEM image prune -f

if [[ ! -d llvm-libc++_build || ! -d musl_libc_build ]]; then
    if [[ $PODMAN == 0 ]]; then
    $CONTAINER_SYSTEM build -f $CUR_DIR/Containerfile --tag clyth_alpine_container:1.0 -v $CUR_DIR/libs_build/:/home/libs_build/:Z --format docker
    else
        $CONTAINER_SYSTEM build -f $CUR_DIR/Containerfile --tag clyth_alpine_container:1.0 -v $CUR_DIR/libs_build/:/home/libs_build/:Z
    fi

    if [[ $? != 0 ]]; then
        printf "Error occurred during podman build call - exiting\n"
        $CONTAINER_SYSTEM image prune -f
        exit 1
    fi
    $CONTAINER_SYSTEM rmi localhost/clyth_alpine_container:1.0
    $CONTAINER_SYSTEM container prune --force
    $CONTAINER_SYSTEM image prune -f
fi

cd $CUR_DIR
CMAKE_LISTS_FILE_PATH=./compiler-src/
rm -rf build/
cmake -S $CMAKE_LISTS_FILE_PATH -Bbuild
cd build/
make
#!/bin/bash
CUR_DIR=$(dirname $0)
cd $CUR_DIR

CMAKE_LISTS_FILE_PATH=../compiler-src/CMakeLists.txt
mkdir -p build/
cd build/

cmake $CMAKE_LISTS_FILE_PATH

#!/bin/bash

BACKEND_DIR="../backend"
BUILD_DIR="../builds"
rm ${BUILD_DIR}/backend-binary

clang++ -stdlib=libc++ -std=c++17 ${BACKEND_DIR}/*.cpp -o ${BUILD_DIR}/backend-binary


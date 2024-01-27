#!/bin/bash

FRONTEND_DIR="../frontend"
BUILD_DIR="../builds"

clang++-17 -stdlib=libc++ -std=c++20 ${FRONTEND_DIR}/*.cpp -o ${BUILD_DIR}/frontend-binary


#!/bin/bash

FRONTEND_DIR="../frontend"
BUILD_DIR="../builds"
rm ${BUILD_DIR}/frontend-binary

clang++ -stdlib=libc++ -std=c++17 ${FRONTEND_DIR}/*.cpp -o ${BUILD_DIR}/frontend-binary


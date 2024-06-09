#!/bin/bash

CPP_FILES=$(ls *.cpp)

clang++ -std=c++23 -stdlib=libc++ $CPP_FILES -o test.bin
./test.bin
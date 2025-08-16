#!/bin/bash

CPP_FILES=$(ls *.cpp)

clang++ -std=c++17 -stdlib=libc++ $CPP_FILES -o clythcpp

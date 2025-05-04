#!/bin/bash

GRAMMAR_FILE=$1
RULE=$2
CLYTH_FILE=$3

if [[ $# < 3 ]]; then
    echo "Need three args for viewing gui: grammar-file rule clythfile"
    exit 1
fi

antlr4-parse $GRAMMAR_FILE $RULE -gui $CLYTH_FILE
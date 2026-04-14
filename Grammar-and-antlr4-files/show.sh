#!/bin/bash
GRAMMAR_FILE=$1
GRAMMAR_RULE=$2
MODE=$3
INPUT_FILE=$4

if [[ $MODE != "-gui" ]] && [[ $MODE != '-tree' ]]; then
echo $MODE
    printf "\nERROR, mode should either be -gui or -tree.\nExiting...\n"
    exit 1
fi

antlr4-parse $GRAMMAR_FILE $GRAMMAR_RULE $MODE $INPUT_FILE
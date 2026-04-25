#!/bin/bash
GRAMMAR_FILE=$1
GRAMMAR_RULE="program"
MODE=$2
INPUT_FILE=$3

if [[ $MODE != "-gui" ]] && [[ $MODE != '-tree' ]]; then
    MODE="-gui"
fi

antlr4-parse $GRAMMAR_FILE $GRAMMAR_RULE $MODE $INPUT_FILE
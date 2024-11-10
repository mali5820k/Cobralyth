#!/bin/bash

antlr4-parse clyth_generated_parser/clyth_grammar.g4 program $1 -tree

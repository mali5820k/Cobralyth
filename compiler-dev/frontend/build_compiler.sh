#!/bin/bash
ENTRYPOINT="./src/main.ts"
TESTPROGRAM="./clyth_programs/testprogram.clyth"

bun build $ENTRYPOINT --target=bun --outdir ./test_build/
#bun run ./test_build/main.js --help
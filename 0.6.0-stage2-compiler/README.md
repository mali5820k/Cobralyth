# Clyth 0.6.0 Stage 2 compiler

This directory owns the compiler sources compiled by the Stage 1 compiler.
`build_stage2.sh` produces the only release tarball in the staged bootstrap flow:

- `../0.6.0-compiler-dist/`
- `../clyth-0.6.0-compiler-dist.tar.gz`

The current `src/` files preserve the minimum bridge compiler surface. They are
the replacement point for the real self-hosted lexer, Pratt parser, semantic
passes, and LLVM IR emitter as those pieces mature.

# Clyth 0.6.0 Stage 1 bootstrap

This tree intentionally omits CASM and serialized AST output.

Stage 1 is the smallest useful Clyth-written compiler driver. The frozen 0.5.0
compiler remains the backend while the new driver owns command-line behavior and
selects an independently built runtime module store. This proves that new Clyth
tooling can be built by Stage 0 without mutating the frozen 0.5.0 source/runtime
tree.

The standalone `clyth-module` program builds and installs modules by convention:

- declarations: `clyth-runtime/modules/module-<name>/`
- optional binding: `clyth-runtime/c-bindings/<name>/<name>.c`
- optional flags: `clyth-runtime/c-bindings/<name>/build.flags`
- installed store: `module-store/modules/module-<name>/`

The next Stage 1 iterations replace delegation in this order:

1. import/module resolution
2. trie lexer and token stream
3. Pratt parser
4. semantic analysis
5. direct LLVM backend

Only after the new compiler can compile revised source should Stage 2 adopt the
full 0.6.0 syntax and substantially redesign the compiler and ecosystem.

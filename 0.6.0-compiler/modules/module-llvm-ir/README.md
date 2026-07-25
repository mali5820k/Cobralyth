# Clyth LLVM IR Module

This module is the migration boundary between the Clyth frontend and LLVM.
It emits textual LLVM IR and has no dependency on LLVM's C++ API, libc++,
`fmt`, `cxxopts`, or ANTLR4.

The initial surface covers modules, targets, primitive and aggregate types,
values, declarations, function boundaries, basic blocks, memory operations,
arithmetic/call instructions, returns, and branches. Migration should happen
one emitter family at a time while the existing 25-test regression suite stays
green.

The current `bootstrapping-stage1` dependency exists only for string assembly.
It should disappear when the native Clyth string builder is ready.

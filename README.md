# Clyth

Clyth is an LLVM-backed ahead-of-time systems programming language project focused on a small compiler architecture, explicit runtime boundaries, and native Linux artifacts built through a musl/libc++ toolchain path.

The current compiler lowers Clyth programs through LLVM IR generation, using LLVM as a mature optimization and native-code backend while keeping Clyth's language design, runtime model, and long-term compiler architecture independent.

Alpha 0.5.0 is the **Runtime Ecosystem Foundation Release**. It moves Clyth beyond a compiler experiment by proving that Clyth code can compile to native executables, cross stable ABI boundaries, and drive useful runtime capabilities such as file I/O, JSON, hashing, and a libuv-backed server runtime.

Documentation: https://mali5820k.github.io/clyth-docs/

---

## Why Clyth Exists

Clyth explores a systems-language design that keeps low-level control explicit while avoiding unnecessary ceremony around runtime capability. The compiler should own syntax, typing, semantic analysis, and lowering. Runtime capabilities should live behind Clyth-facing APIs and small, auditable native ABI boundaries.

The long-term direction is a language capable of writing systems applications, backend services, embedded-facing runtime code, tooling, and eventually its own compiler/runtime ecosystem.

Core principles:

- Keep the compiler focused.
- Keep runtime APIs ergonomic.
- Keep native boundaries explicit and auditable.
- Prefer permissively licensed dependencies.
- Avoid binding the language identity to a single backend implementation.
- Use MECC later for deterministic estate-based ownership once the compiler architecture is ready.

---

## 0.5.0 Status

Clyth currently compiles the supported alpha language subset into native Linux executables through LLVM IR and a Zig/musl-based link path.

The 0.5.0 release candidate validates:

- structs, methods, constructors, and explicit `this`
- extension-style method declarations
- first-class function values and `function<Return(Args...)>` syntax
- lambda callbacks
- expression-based template interpolation
- native string values and string return ABI
- runtime printing through `print` / `println`
- libuv-backed file I/O
- yyjson-backed JSON runtime integration
- rapidhash-backed hashing
- HTTPS/server abstraction with plain HTTP when TLS certificates are not configured
- router callbacks using `Request` and `Response` wrappers
- stable handle-based ABI boundaries
- recursive artifact audit for GNU/glibc/libstdc++ leakage

The regression suite currently passes 20/20 samples, and the ABI audit reports no GNU/glibc/libstdc++ markers in checked release artifacts.

---

## Architecture Summary

Clyth separates user-facing APIs from native implementation details:

```text
Clyth application
    ↓
Clyth wrapper module
    ↓
Stable ABI boundary
    ↓
Native provider
    ↓
libuv / yyjson / rapidhash / OS
```

For example, the server API exposes Clyth `Request` and `Response` values while the native runtime stores request state behind handles. This keeps callback ABIs stable without forcing application code to depend on raw native layouts.

Detailed architecture and module documentation live in the documentation site rather than this README.

---

## MECC Memory Model Direction

MECC (**Managed Entanglement for Collapsible Collections**) is Clyth's planned deterministic memory-management model.

MECC explores a middle ground between fully manual allocation management and runtime-driven memory systems. The goal is to preserve predictable ownership behavior while reducing the amount of repetitive lifetime management required in larger systems.

Instead of treating every allocation as an isolated lifetime problem, MECC introduces ownership estates: groups of related allocations whose lifetimes can be analyzed, optimized, and cleaned up together.

The planned MECC analysis pipeline focuses on:

- detecting ownership relationships
- grouping related allocations into logical estates
- reducing unnecessary object-level lifetime bookkeeping
- automatically inserting cleanup paths where ownership is clear
- falling back to explicit estate management when static analysis cannot prove safe behavior

MECC does not attempt to hide memory management. It attempts to move ownership reasoning to a higher level where the compiler and developer can cooperate.

Clyth 0.5.0 intentionally focuses on building realistic runtime foundations first — files, requests, responses, collections, callbacks, and module boundaries — so future MECC analysis operates against real application patterns rather than artificial examples.

---

## Building

Build the compiler:

```bash
./build_compiler.sh
```

Fast development build without packaging:

```bash
./build_compiler.sh --skip-dist
```

Run the regression suite and ABI audit:

```bash
./run_regression_tests.sh
./audit_no_gnu_abi.sh
```

---

## Distribution Policy

Clyth release artifacts should remain lean and auditable. Distribution bundles should include the compiler binary, runtime files, samples, documentation, inspection tools, and license files.

They should not include:

- LLVM source trees
- LLVM build directories
- Zig toolchains
- generated object caches
- unrelated vendor source checkouts

The release audit should reject accidental GNU/glibc/libstdc++ runtime linkage in checked artifacts.

---

## Roadmap

### 0.5.0 — Runtime Ecosystem Foundation

Lock the current alpha language/runtime surface, prove the Clyth wrapper → ABI → native provider model, and release a clean musl/libc++-audited toolchain artifact.

### 0.6.0 — Compiler Simplification and Bootstrap Path

Begin transitioning Clyth from a C++ bootstrap compiler toward a compiler implemented in Clyth itself.

Primary goals:

- replace ANTLR with a compact Pratt parser
- implement Clyth-owned AST and IR structures
- create a Clyth LLVM IR emission library
- reduce dependency on LLVM's C++ APIs
- keep LLVM optimization/code generation as the backend pipeline
- preserve the existing musl/libc++ artifact strategy

### Later Milestones

- support standalone x86_64 and aarch64 Linux musl targets
- expand module build/distribution tooling
- introduce MECC ownership analysis
- move toward a self-hosted compiler and runtime ecosystem

---

## AI-Assisted Development Philosophy

Clyth is AI-assisted, not AI-authored.

AI-assisted engineering tools have been used during development to accelerate implementation, debugging, refactoring, build-system iteration, and exploration of compiler/runtime design alternatives. The language design, architecture, roadmap, syntax direction, runtime policy, release decisions, and final code review remain human-directed.

This project documents that workflow openly because tooling used during development should not be confused with authorship, design ownership, or release responsibility.

---

## Legal Notes

Clyth prefers permissively licensed dependencies and runtime pieces.

Preferred external dependency licenses include MIT, BSD-2-Clause, BSD-3-Clause, Zlib, and Apache-2.0 when explicitly tracked.

Avoided unless absolutely necessary: GPL, LGPL, AGPL, or other licenses that complicate static distribution.

Third-party project names are used to describe architecture and dependency relationships. Clyth is not affiliated with, endorsed by, or sponsored by those projects or their maintainers.

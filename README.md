# Clyth

Clyth is a systems programming language project that currently targets
LLVM IR for ahead-of-time native code generation. The project focuses on
a small compiler architecture, explicit runtime boundaries, and native
Linux artifacts built through a musl-oriented build pipeline.

The current compiler lowers Clyth programs through LLVM IR generation,
using LLVM as a mature optimization and native-code backend while
keeping Clyth's language design, runtime model, and long-term compiler
architecture independent.

Alpha 0.5.0 is the **Runtime Ecosystem Foundation Release**. It moves
Clyth beyond a compiler experiment by proving that Clyth code can
compile to native executables, cross stable ABI boundaries, and drive
useful runtime capabilities such as file I/O, JSON, hashing, and a
libuv-backed server runtime.

## Documentation

Official documentation:

https://mali5820k.github.io/clyth-docs/

------------------------------------------------------------------------

## Why Clyth Exists

Clyth explores a systems-language design that keeps low-level control
explicit while avoiding unnecessary ceremony around runtime capability.
The compiler should own syntax, typing, semantic analysis, and lowering.
Runtime capabilities should live behind Clyth-facing APIs and small,
auditable native ABI boundaries.

The long-term direction is a language capable of writing systems
applications, backend services, embedded-facing runtime code, tooling,
and eventually its own compiler/runtime ecosystem.

Core principles:

-   Keep the compiler focused.
-   Keep runtime APIs ergonomic.
-   Keep native boundaries explicit and auditable.
-   Prefer permissively licensed dependencies.
-   Avoid binding the language identity to a single backend
    implementation.
-   Use MECC later for deterministic estate-based ownership once the
    compiler architecture is ready.

------------------------------------------------------------------------

## 0.5.0 Status

Clyth currently compiles the supported alpha language subset into native
Linux executables through LLVM IR and a musl-oriented native build
pipeline.

The 0.5.0 Alpha release includes:

-   structs, methods, constructors, and explicit `this`
-   extension-style method declarations
-   first-class function values and `function<Return(Args...)>` syntax
-   lambda callbacks
-   expression-based template interpolation
-   native string values and string return ABI
-   runtime printing through `print` / `println`
-   libuv-backed file I/O
-   yyjson-backed JSON runtime integration
-   rapidhash-backed hashing
-   HTTPS/server abstraction with plain HTTP when TLS certificates are
    not configured
-   router callbacks using `Request` and `Response` wrappers
-   stable handle-based ABI boundaries
-   recursive artifact audit for GNU/glibc/libstdc++ leakage

The regression suite currently passes **24/24** samples, and the ABI
audit reports no GNU/glibc/libstdc++ markers in checked release
artifacts.

------------------------------------------------------------------------

## Architecture Summary

Clyth separates user-facing APIs from native implementation details:

``` text
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

Detailed architecture and module documentation live in the documentation
site rather than this README.

------------------------------------------------------------------------

## MECC Memory Model Direction

MECC (**Managed Entanglement for Collapsible Collections**) is Clyth's
planned deterministic memory-management model. Clyth 0.5.0 intentionally
focuses on building realistic runtime foundations first so future MECC
analysis operates against real application patterns rather than
artificial examples.

------------------------------------------------------------------------

## Building

``` bash
./build_compiler.sh
```

Fast development build:

``` bash
./build_compiler.sh --skip-dist
```

Regression suite:

``` bash
./run_regression_tests.sh
./audit_no_gnu_abi.sh
```

------------------------------------------------------------------------

## Roadmap

### 0.6.0 --- Self-Hosted Compiler

Begin replacing the transitional C++ compiler with a compiler written in
Clyth.

Primary goals:

-   rewrite the compiler in Clyth
-   replace ANTLR with a Pratt parser
-   implement a Clyth-owned AST and IR
-   emit LLVM IR directly
-   reduce dependence on LLVM's C++ libraries
-   preserve the musl-oriented native toolchain

------------------------------------------------------------------------

## AI-Assisted Development Philosophy

Clyth is AI-assisted, not AI-authored.

AI-assisted engineering tools have been used during development to
accelerate implementation, debugging, refactoring, build-system
iteration, and exploration of compiler/runtime design alternatives.
Language design, architecture, roadmap, release decisions, and final
review remain human-directed.

------------------------------------------------------------------------

## Legal Notes

Clyth prefers permissively licensed dependencies. Third-party project
names are referenced only to describe architecture and dependency
relationships and do not imply endorsement or affiliation.

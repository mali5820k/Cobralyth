# Cobralyth, Clyth for short

## Current Status:
The Clyth frontend is currently being explored between an Antlr4 frontend implementation and a C++ Pratt Parser implementation. This decision may swing in favor of Antlr4 if project time and effort exceeds the benefits of maintaining a custom parser.

The base-language is designed around manually managed memory similar to C, while the MECC module (available separately) is an optional static-analysis optimizer intended to introduce deterministic ownership and memory reclamation strategies at compile time.

The project is evolving and the README may be updated to reflect the latest design decisions.

---

# About Clyth

Clyth is an ahead-of-time (AOT) compiled systems programming language implemented in C++ with LLVM IR code generation.

The language is designed as an opinionated iteration of C focused on:
- explicit control
- predictable performance
- improved ergonomics for systems development
- LLVM-based optimization and portability

The long-term goal of Clyth is to provide a cleaner systems-programming experience without sacrificing the low-level control and runtime predictability expected from C-family languages.

Clyth supports manual memory management by default, allowing developers to retain full ownership over allocation and reclamation behavior when desired.

In addition to the base language, the project includes an optional static-analysis optimization framework called MECC (Managed Entanglement for Collapsable Collections).

Rather than relying on tracing garbage collection, MECC is designed around deterministic ownership analysis, lifetime propagation, alias cleanup, container-aware ownership propagation, and selective memory management strategies such as:
- single ownership patterns
- shared reference counting
- ARC-like ownership behavior
- weak references for non-owning relationships
- arena-style allocation optimizations where static analysis determines they are beneficial

The goal of MECC is to automatically transform eligible Clyth programs into deterministic memory-management patterns while preserving systems-level performance characteristics without tracing garbage collection or stop-the-world pauses.

Current project work is focused on:
- parser architecture
- AST generation
- semantic analysis foundations
- LLVM IR lowering
- runtime design

---

# Project Structure

You will find the project split into sub-projects that (with the exception of "antlr4-cpp-runtime-src") all fall under the main MIT license specified in the repository:

## antlr4-cpp-runtime-src
- Bundled library containing the Antlr4 C++ runtime source code versioned 4.13.2.
- Built using the Zig C/C++ toolchain.
- License information is included in:
  - `antlr4-cpp-runtime-src/LICENSE.txt`
  - `EXTERNAL_LIBRARIES_LICENSES.md`

## compiler-src
- Main C++ compiler frontend/backend project.
- Responsible for:
  - parsing
  - AST generation
  - semantic analysis
  - LLVM IR generation
  - binary production

## clyth-runtime
- Runtime and standard-library project.
- Intended to provide:
  - file I/O
  - collections
  - threading utilities
  - runtime helpers
  - future networking support

As with all long-term projects, directory structures and organization may evolve over time.

---

# Goals and Aspirations

## Primary Goals

- Simplified systems-programming ergonomics.
- Cleaner syntax compared to traditional C pointer-heavy semantics.
- LLVM IR compilation pipeline.
- Built-in collection syntax support.
- Deterministic memory-management optimization through MECC.
- Bootstrappable systems language foundations.
- C interoperability.

## Future Aspirations

- Package manager and build system.
- JSON support.
- GUI support.
- Websocket and webserver utilities.
- Embedded and IoT support.
- GPU and MLIR exploration.
- WASM support through non-browser runtimes.
- Language server implementation.
- Debugger tooling.

---

# Clyth Language Spec V1

## Core Language Characteristics

- Ahead-of-Time (AOT) compiled.
- LLVM IR backend.
- Manual memory management by default.
- Pass-by-reference semantics for objects.
- Pass-by-value semantics for primitive types.
- Optional semicolons.
- C interoperability through `extern C`.
- Built-in collections syntax.

---

# Example Syntax

## Structs and Manual Memory

```go
struct Person {
    string name,
    int32 age,
}

int32 main() {
    Person p = malloc(Person)

    p.name = "Harry"
    p.age = 30

    print(p.name)

    free(p)

    return 0
}
```

---

## Lists

```go
int32[] values = [1, 2, 3, 4]
```

---

## Fixed Arrays

```go
int32[10] fixed_values = []
```

---

## Maps

```go
numeric:string sample_map = {
    1: "one",
    2: "two",
    1000: "thousand",
}
```

---

## Sets

```go
int32() unique_values = {
    1, 2, 3, 4, 5
}
```

---

## C Interoperability

```c
extern C int32 printf(string fmt, ...)
```

---

## Type Relationships

```py
if instance is drawable {
    print("Drawable instance")
}
```

---

# Base Clyth Memory Model

The base Clyth language relies on manually managed memory similar to C.

This ensures:
- low-level systems control
- predictable allocation behavior
- interoperability with existing native tooling
- compatibility with environments where deterministic control is required

Core allocation primitives include:
- `malloc`
- `free`

Future runtime abstractions may build on top of these primitives while preserving explicit systems-level control.

---

# MECC Overview

## Managed Entanglement for Collapsable Collections

MECC is a separate static-analysis optimization framework for Clyth.

Its purpose is to analyze ownership relationships, lifetimes, aliases, containers, and escape patterns in order to transform eligible code into deterministic memory-management patterns.

MECC is designed to avoid tracing garbage collection approaches such as:
- mark-and-sweep
- stop-the-world tracing
- periodic heap scanning

Instead, MECC explores deterministic ownership and reclamation strategies such as:
- ownership propagation
- alias cleanup
- reference counting
- ARC-like ownership semantics
- weak references for non-owning relationships
- container-aware ownership propagation
- opportunistic arena-style allocation optimizations

The goal is to preserve systems-level performance characteristics while reducing manual ownership orchestration complexity for developers.

---

# Example MECC Alias Cleanup

Source code:

```go
int32 main(string[] args) {
    string[] alias1 = args
    string[] alias2 = alias1
    string[] alias3 = alias2

    print(alias3)

    return 0
}
```

Potential optimized transformation:

```go
int32 main(string[] args) {
    print(args)

    return 0
}
```

This optimization removes unnecessary aliases while preserving observable program behavior.

---

# Acknowledgements and Legal Notes

- The Zig compiler toolchain is leveraged for portable static-linking support.
- musl-libc and LLVM libc++ are preferred over glibc/libstdc++ for portability and licensing preferences.
- External library licensing information is documented in:
  - `EXTERNAL_LIBRARIES_LICENSES.md`

## Legal Disclaimer

The author of Clyth is not legally responsible for ensuring license compliance for programs written using the language or its tooling.

Developers are responsible for verifying compliance with all third-party licenses included in their final binaries or distributions.

Consult legal professionals where appropriate.


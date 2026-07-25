# Clyth

Clyth is a systems programming language designed for native software development with an emphasis on explicit runtime boundaries, deterministic tooling, and self-hosting.

The compiler currently lowers Clyth source into LLVM IR before producing native executables through a musl-oriented toolchain. LLVM serves as the optimization and code-generation backend while the language, runtime, compiler architecture, and tooling remain independent of any single backend implementation.

Version 0.6.0 focuses on completing the transition to a self-hosted compiler while preserving a small, understandable compiler architecture.

## Documentation

Official documentation:

https://mali5820k.github.io/clyth-docs/

---

# Why Clyth Exists

Clyth explores a systems-language design that keeps low-level control explicit while reducing unnecessary complexity around runtime capability.

The compiler owns parsing, semantic analysis, type checking, optimization, and code generation. Runtime functionality is exposed through Clyth-native APIs with small, auditable ABI boundaries.

The long-term goal is a language capable of building:

- Systems software
- Backend services
- Networking applications
- Developer tooling
- Embedded-oriented runtime components
- Its own compiler and ecosystem

Core principles:

- Small compiler architecture
- Explicit runtime boundaries
- Ergonomic native APIs
- Permissively licensed dependencies
- Backend independence
- Deterministic tooling

---

# Current Capabilities

Current language and runtime support includes:

- Structures, methods, constructors, and explicit `this`
- Extension methods
- First-class function values
- Lambda expressions
- Native strings
- Template string interpolation
- File I/O
- JSON support
- Hashing
- libuv networking
- HTTP/HTTPS server abstractions
- Stable C ABI integration
- Recursive artifact auditing for GNU ABI leakage

Additional language features continue to be implemented as part of the 0.6.0 self-hosting effort.

---

# Repository Layout

The repository is organized around the compiler bootstrap process.

```text
bootstrap/
├── 0.5.0-compiler/
│   ├── src/
│   ├── modules/
│   ├── runtime/
│   ├── scripts/
│   ├── build-dependencies/
│   └── dist/
│
├── 0.5.1-compiler/
│   ├── src/
│   ├── modules/
│   ├── runtime/
│   ├── scripts/
│   ├── build-dependencies/ -> ../0.5.0-compiler/build-dependencies
│   └── dist/
│
├── shared-license-files/
├── scripts/
└── build_driver.sh

0.6.0-compiler/
├── src/
├── modules/
├── runtime/
├── scripts/
└── dist/

build/
```

## Bootstrap Stages

### 0.5.0 Compiler

The trusted bootstrap compiler.

This compiler serves as the initial seed used to begin the self-hosting process.

---

### 0.5.1 Compiler

The intermediate bootstrap compiler.

Built using the 0.5.0 compiler, it exists solely to validate that Clyth can successfully compile itself before producing the final compiler.

---

### 0.6.0 Compiler

The final bootstrapped compiler.

Unlike the bootstrap compilers, it owns its runtime modules, generated license manifest, tests, and distributable artifacts independently.

---

# Building

Build the complete bootstrap chain:

```bash
./build_bootstrapped_compiler_from_toolchain.sh
```

The bootstrap driver:

1. Builds the 0.5.1 compiler
2. Validates the 0.5.1 compiler
3. Builds the 0.6.0 compiler
4. Validates the final compiler
5. Produces the distributable compiler

The installation scripts consume only the final compiler distribution.

---

# Roadmap

The primary goals for the 0.6.0 release are:

- Complete the self-hosted compiler
- Replace ANTLR with a Pratt parser
- Implement a Clyth-owned parser, AST, semantic pipeline, and LLVM IR generation
- Continue reducing compiler-side C++ dependencies
- Preserve the musl-oriented toolchain
- Expand the standard library
- Continue development of **MECC (Managed Entanglement for Collapsible Collections)**

Future releases will focus on tooling, editor integration, package management, improved diagnostics, and broader platform support once the compiler has fully bootstrapped itself.

---

# AI-Assisted Development

Clyth is developed with AI-assisted engineering tools used for implementation, debugging, refactoring, and design exploration.

Architecture, language design, implementation decisions, and final review remain human-directed.

---

# Legal Notes

Clyth prefers permissively licensed dependencies. Third-party project names are referenced solely to describe interoperability, architecture, or dependency relationships and do not imply endorsement or affiliation.
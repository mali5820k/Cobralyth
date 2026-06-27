# Clyth

Clyth is an LLVM-based ahead-of-time systems programming language implemented in C++.

Clyth's design goal is simple: keep the language small, keep the runtime powerful, and keep the compiler focused on understanding and lowering the language instead of owning every high-level feature directly.

Alpha 0.3.0 is the **Foundation Release**. It stabilizes the language/runtime boundary, introduces the runtime module layout, consolidates containers around array-backed runtime components, and establishes the shape of future Clyth modules.

---

## Why Clyth Exists

Clyth explores a systems-language design that preserves explicit control and predictable performance while reducing unnecessary C/C++ ceremony.

The long-term goal is a language that can write real systems applications, web services, embedded-facing runtime code, tooling, and eventually its own compiler/runtime ecosystem without forcing every feature into compiler internals.

Clyth is designed around these principles:

- Programs should express intent clearly.
- Arrays and strings are core language primitives.
- Containers and services should live in Clyth-space runtime modules.
- The compiler should own syntax, typing, lowering, and analysis.
- Runtime capabilities should be installable, replaceable, and license-trackable.
- Memory management should eventually support deterministic estate-based ownership through MECC.

---

## Current Status

Clyth currently compiles the Alpha 0.3.0 language subset into native Linux executables through LLVM IR and Zig/musl-based static linking.

Implemented or scaffolded areas include:

- ANTLR4 lexer/parser frontend.
- AST generation.
- Semantic analysis infrastructure.
- Linear lowering plan.
- LLVM IR generation for the current Alpha subset.
- `extern C` declarations and calls.
- `int32 main(string[] args)` command-line argument support.
- Local variables, global variables, user functions, function hoisting, and global hoisting.
- `if`, `else`, `while`, `break`, and `continue`.
- Fixed arrays and compact dynamic arrays.
- Native string values.
- Structs, fields, methods, constructors, and implicit `this`.
- Runtime container scaffolds for `List<T>`, `Set<T>`, and `Map<K,V>`.
- Keyed-array syntax for map-style initialization.
- Runtime modules under `clyth-runtime/modules/`.
- C binding source separation under `clyth-runtime/c-bindings/`.
- DMA runtime module with C-backed static archive linking.
- JSON, web, and TLS runtime module stubs.
- Runtime module metadata and architecture-aware C binding metadata.
- Program-specific license bundle groundwork.
- Sample smoke-test runner.
- Distribution packaging with compiler binary, runtime files, samples, and licenses.

---

## Design Boundary

Alpha 0.3.0 locks in an important architectural boundary.

Compiler-backed:

```text
arrays
strings
structs
functions
generic syntax
keyed-array literals
lowering
LLVM IR generation
```

Runtime-owned:

```text
List<T>
Set<T>
Map<K,V>
JSON
web/TLS wrappers
DMA APIs
estates
allocation policies
module packaging
```

The compiler should only own what cannot reasonably be implemented inside the language/runtime itself.

---

## Language Examples

### Hello World

```clyth
extern C int32 printf(string fmt, ...)

int32 main() {
    printf("Hello from Clyth!\n")
    return 0
}
```

### Command-Line Arguments

```clyth
extern C int32 printf(string fmt, ...)

int32 main(string[] args) {
    printf("program: %s\n", args[0])
    return 0
}
```

### Structs, Constructors, Methods, and `this`

```clyth
extern C int32 printf(string fmt, ...)

struct Counter {
    private int32 value

    constructor() {
        value = 0
    }

    void add(int32 amount) {
        value += amount
    }

    int32 get() {
        return value
    }
}

int32 main() {
    Counter counter
    counter.add(42)
    printf("counter: %d\n", counter.get())
    return 0
}
```

### Compact Arrays

Plain `T[]` arrays are compact primitives. They track initialized elements through `.length`. Capacity belongs to growable runtime containers, not plain arrays.

```clyth
extern C int32 printf(string fmt, ...)

int32 main() {
    int32[] values = [10, 20, 30]
    values[1] = 42

    printf("length: %d\n", values.length)
    printf("middle: %d\n", values[1])
    return 0
}
```

Conceptual layout:

```text
T[]
  data
  length
```

### Native Strings

```clyth
extern C int32 printf(string fmt, ...)

int32 main() {
    string message = "hello clyth"
    printf("message: %s\n", message)
    printf("length: %d\n", message.length)
    printf("first char code: %d\n", message[0])

    message = "strings are native now"
    printf("updated: %s\n", message)
    return 0
}
```

### Lists

`List<T>` is the growable runtime container layer. Capacity and growth policy belong here.

```clyth
extern C int32 printf(string fmt, ...)

int32 main() {
    List<int32> values = [3, 4]
    values.push(35)

    printf("length: %d\n", values.length)
    printf("capacity: %d\n", values.capacity)
    printf("last: %d\n", values[2])

    int32 popped = values.pop()
    printf("popped: %d\n", popped)

    values[1] += 38
    printf("middle after add: %d\n", values[1])
    return 0
}
```

Long-term runtime direction: `List<T>` should use head/tail indices over a backing array where possible, enabling efficient front/back operations without forcing full-array shifts for every pop/shift-style operation.

### Sets

`Set<T>` is a runtime container built on the same array/list storage foundation, with duplicate prevention.

```clyth
extern C int32 printf(string fmt, ...)

int32 main() {
    Set<int32> values = [3, 4]
    values.insert(42)
    values.insert(42)

    printf("set length: %d\n", values.length)
    printf("contains 42: %d\n", values.contains(42))
    printf("contains 99: %d\n", values.contains(99))
    return 0
}
```

### Maps and Keyed Arrays

Maps use normal `Map<K,V>` generic syntax and keyed-array initialization. The keyed array is a language-level collection literal; the Map behavior belongs in the runtime.

```clyth
extern C int32 printf(string fmt, ...)

int32 main() {
    Map<int32, int32> scores = [
        3: 30,
        4: 40,
        42: 100
    ]

    scores.put(4, 44)

    printf("value 3: %d\n", scores.get(3))
    printf("value 4: %d\n", scores.get(4))
    printf("has 99: %d\n", scores.contains_key(99))
    return 0
}
```

Long-term constructor direction:

```clyth
Map<string, int32> scores = new Map([
    "one": 1,
    "two": 2
])
```

The compiler recognizes keyed-array literals and the runtime owns the Map implementation.

---

## Runtime Modules

Clyth runtime functionality is organized under `clyth-runtime/`.

```text
clyth-runtime/
├── c-bindings/
│   ├── dma/
│   ├── libwebsockets/
│   └── openssl/
├── modules/
│   ├── module-collections/
│   ├── module-dma/
│   ├── module-json/
│   ├── module-web/
│   └── module-tls/
├── runtime_libraries.json
└── EXTERNAL_LIBRARIES_LICENSES.md
```

The `modules/` directory is the single source of truth for Clyth runtime module source and metadata. C binding source code remains under `c-bindings/`.

`runtime_libraries.json` is a compiler-consumed index/cache. Future `clyth install`, `clyth uninstall`, and `clyth rebuild-index` commands should regenerate it from module metadata rather than expecting developers to edit it directly.

---

## Module Metadata

Each runtime module owns a `module.json` file. Clyth wrapper code and native C bindings are tracked separately so the compiler can eventually generate accurate license bundles for compiled programs.

Example shape:

```json
{
  "name": "dma",
  "version": "0.3.0-alpha",
  "kind": "runtime-module",
  "exports": [
    "dma.clyth"
  ],
  "wrapper": {
    "license": "MIT"
  },
  "c_bindings": [
    {
      "name": "clyth_dma",
      "enabled": true,
      "backend": "Clyth DMA C Binding",
      "license": "MIT",
      "source_directory": "../../c-bindings/dma",
      "architectures": {
        "x86_64": {
          "archive": "x86_64/libclyth_dma.a"
        }
      }
    }
  ]
}
```

This format is intentionally architecture-aware so modules can eventually ship prebuilt archives or LLVM IR for multiple targets.

---

## DMA Runtime Module

DMA/MMIO is treated as runtime-backed functionality, not as raw pointer syntax.

Clyth code passes integer addresses to explicit runtime functions. The C binding owns the ugly platform-specific implementation details.

```clyth
include "dma/dma.clyth"

int32 main() {
    // Future embedded-facing code can call dma_read32/dma_write32 here.
    return 0
}
```

This keeps C pointer baggage out of the language surface while still allowing systems and embedded escape hatches through runtime modules.

---

## JSON, Web, and TLS Direction

Alpha 0.3.0 adds stubs and module metadata for future JSON, web, and TLS work.

- JSON is planned as a native Clyth implementation using strings, lists, maps, arrays, and keyed arrays.
- The web runtime is planned as an ExpressJS-like Clyth-space API with replaceable C backends.
- TLS is planned as a runtime module, with OpenSSL tracked as an Apache-2.0 backend candidate when enabled.

The public APIs should remain Clyth-owned even if lower-level libraries are used behind the scenes.

---

## Building

Build the compiler:

```bash
./build_compiler.sh
```

Fast development build without packaging the distribution:

```bash
./build_compiler.sh --skip-dist
```

Compile a sample program:

```bash
./build-compiler/clyth_compiler_bin -c sample-clyth-programs/printf_test.clyth -o printf_test
./printf_test
rm -f printf_test
```

Emit LLVM IR only:

```bash
./build-compiler/clyth_compiler_bin -c sample-clyth-programs/printf_test.clyth -o printf_test --emit-ir-only
```

Preserve generated IR while also linking an executable:

```bash
./build-compiler/clyth_compiler_bin -c sample-clyth-programs/printf_test.clyth -o printf_test --emit-ir
```

Show bundled/project license text:

```bash
./build-compiler/clyth_compiler_bin --show-licenses
```

---

## Testing

Run the language smoke-test suite:

```bash
./build_compiler.sh --skip-dist
./sample-clyth-programs/run_language_samples.sh
rm -rf sample-clyth-programs/.sample-bin
```

The test runner compiles each sample, runs the produced binary, checks expected stdout, prints a visible `[ OK ]` or `[ FAILED ]` result for every sample, and removes generated binaries after the run.

---

## Distribution

The build script can create:

```text
dist-clyth/
clyth-dist.tar.gz
```

The distribution includes the compiler binary, sample programs, runtime files, useful LLVM tools, and license files.

Zig is expected to remain installed on the host PATH for final executable linking.

---

## Future CLI Direction

The long-term user-facing command should be `clyth`, with compiler and package-management behavior dispatched by subcommands.

Planned shape:

```bash
clyth compile app.clyth -o app
clyth install module-dma.tar.gz
clyth uninstall dma
clyth list
clyth rebuild-index
clyth module build module-dma/
```

Internally, compiler and package-manager code should remain separate even if they are invoked through the same user-facing executable.

---

## MECC Memory Model Direction

MECC means **Managed Entanglement for Collapsible Collections**.

MECC is Clyth's planned deterministic memory-management model. Rather than tracing the whole heap or reference-counting every object individually, MECC groups related allocations into estates and manages lifetime at the estate level.

Current design direction:

- `new Type()` is the normal construction/allocation gateway.
- `estate.new(Type())` explicitly allocates into a specific estate.
- Strong/weak estate relationships are compile-time graph metadata.
- Strong cycles may be rejected or coalesced into shared lifetime groups when safe.
- `malloc`, `free`, and lower-level allocation details are runtime implementation details unless intentionally exposed through low-level modules.

MECC is not fully implemented yet. Alpha 0.3.0 focuses on stabilizing the runtime/module architecture that MECC will eventually use.

---

### Alpha 0.4 Generic Runtime Instantiation Direction

Alpha 0.4 is moving generics into the semantic pipeline rather than treating `List<T>`, `Set<T>`, or `Map<K,V>` as compiler-owned special cases. Generic templates are intended to behave like compile-time type generators:

```text
struct Box<T> { ... }
Box<int32>

semantic analysis discovers Box<int32>
semantic analysis instantiates Box__int32
lowering sees a concrete struct
```

The generic system must apply to ordinary user-defined structs, not only bundled runtime containers. Runtime collections are expected to become normal clients of this same generic-instantiation machinery.

## Roadmap

### Alpha 0.4.0 — The Language Release

Planned focus:

- First-class functions.
- Function references.
- Generic refinements.
- Enums.
- `switch` / `case`.
- Exhaustiveness groundwork.

### Later Releases

Planned areas:

- Thread spawning.
- Thread pools.
- Native JSON parser/writer.
- HTTP/WebSocket runtime.
- TLS runtime.
- Package/module manager.
- MECC estate runtime.
- Debugger and LSP tooling.
- Benchmark applications.

---

## AI-Assisted Development Philosophy

Clyth is AI-assisted, not AI-authored.

AI tools have been used to accelerate implementation, debugging, refactoring, and build-system work. The language philosophy, memory model direction, architecture decisions, and project goals are driven by the project author's design choices.

The goal is not to claim authorship through keystroke count, but through the ideas, constraints, tradeoffs, and engineering decisions that shape the language.

---

## Legal Notes

Clyth prefers permissive licensing for bundled runtime pieces and external dependencies.

Preferred external dependency licenses:

- MIT
- BSD-2-Clause
- BSD-3-Clause
- Apache-2.0 only when appropriate and explicitly tracked

Avoided unless absolutely necessary:

- GPL
- LGPL
- AGPL
- other licenses that complicate static distribution

Runtime and external library license details are tracked in:

```text
clyth-runtime/EXTERNAL_LIBRARIES_LICENSES.md
compiler-src/EXTERNAL_LIBRARIES_LICENSES.md
```

Developers are responsible for verifying license compliance for their distributed applications. Clyth's module metadata and future license-generation tooling are intended to make that process easier.

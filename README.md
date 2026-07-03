# Clyth

Clyth is an LLVM-based ahead-of-time systems programming language implemented in C++.

Clyth's design goal is to keep the language small, keep the runtime capable, and keep the compiler focused on understanding and lowering the language rather than owning every high-level feature directly.

Alpha 0.4.0 is the **Runtime Foundation Release** and is currently being prepared as the first release candidate for this architecture. It moves runtime collections out of compiler-owned backends and into Clyth runtime modules, while preserving compiler ownership of language primitives such as arrays, structs, functions, methods, constructors, generic instantiation, and LLVM lowering.

---

## Why Clyth Exists

Clyth explores a systems-language design that preserves explicit control and predictable performance while reducing unnecessary ceremony around low-level programming.

The long-term goal is a language capable of writing systems applications, web services, embedded-facing runtime code, tooling, and eventually its own compiler/runtime ecosystem without forcing ordinary runtime capabilities into compiler internals.

Clyth is designed around these principles:

- Programs should express intent clearly.
- Arrays and strings are core language primitives.
- Containers and services should live in Clyth-space runtime modules.
- The compiler should own syntax, typing, lowering, and analysis.
- Runtime capabilities should be installable, replaceable, and license-trackable.
- Memory management should eventually support deterministic estate-based ownership through MECC.

---

## Current Status

Clyth currently compiles the Alpha 0.4.0 language subset into native Linux executables through LLVM IR and Zig/musl-based static linking.

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
- Runtime collections implemented as Clyth modules: `List<T>`, `Set<T>`, and `Map<K,V>`.
- Keyed-array syntax for map-style initialization.
- Explicit package-style runtime includes such as `include "collections"`.
- C binding source separation under `clyth-runtime/c-bindings/`.
- DMA runtime module with C-backed static archive linking.
- JSON, web, and TLS runtime module stubs.
- Runtime module metadata and architecture-aware C binding metadata.
- Program-specific license bundle groundwork.
- Sample smoke-test runner.
- Distribution packaging with compiler binary, runtime files, samples, and licenses.

Alpha 0.4.0 release checkpoint: the numbered language regression suite currently protects 11 active samples spanning extern C, arrays, strings, structs/methods, generic structs, explicit package imports, runtime collections, and DMA include plumbing. The concise release summary is maintained in `RELEASE_NOTES.md`; longer guides will move into `docs/` as the documentation site grows.

---

## Design Boundary

Alpha 0.4.0 locks in an important architectural boundary.

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

The compiler should own the semantics required to understand and lower the language. Capabilities that can be expressed as ordinary runtime code should remain outside the compiler.

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
include "collections"

extern C int32 printf(string fmt, ...)

int32 main() {
    List<int32> values = List([3, 4])
    values.push(35)

    printf("length: %d\n", values.length)
    printf("capacity: %d\n", values.capacity)
    printf("last: %d\n", values.get(2))

    int32 popped = values.pop()
    printf("popped: %d\n", popped)

    int32 middle = values.get(1)
    values.set(1, middle + 38)
    printf("middle after add: %d\n", values.get(1))
    return 0
}
```

Long-term runtime direction: `List<T>` should use head/tail indices over a backing array where possible, enabling efficient front/back operations without forcing full-array shifts for every pop/shift-style operation.

### Sets

`Set<T>` is an array-backed runtime container with duplicate prevention. It is implemented in Clyth source rather than through a compiler-owned Set backend.

```clyth
include "collections"

extern C int32 printf(string fmt, ...)

int32 main() {
    Set<int32> values = Set([3, 4])
    values.insert(42)
    values.insert(42)

    printf("set length: %d\n", values.length)
    printf("contains 42: %d\n", values.contains(42))
    printf("contains 99: %d\n", values.contains(99))
    return 0
}
```

### Maps and Keyed Arrays

Maps use normal `Map<K,V>` generic syntax and keyed-array initialization. The keyed array is a language-level literal form; Map behavior belongs in the runtime.

```clyth
include "collections"

extern C int32 printf(string fmt, ...)

int32 main() {
    Map<int32, int32> scores = Map([
        3: 30,
        4: 40,
        42: 100
    ])

    scores.put(4, 44)

    printf("value 3: %d\n", scores.get(3))
    printf("value 4: %d\n", scores.get(4))
    printf("has 99: %d\n", scores.contains_key(99))
    return 0
}
```

Stack allocation uses normal constructor syntax:

```clyth
Map<string, int32> scores = Map([
    "one": 1,
    "two": 2
])
```

Heap allocation uses `new`:

```clyth
Map<string, int32> scores = new Map([
    "one": 1,
    "two": 2
])
```

The compiler recognizes keyed-array literals and constructor calls; the runtime owns the Map implementation.

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

`runtime_libraries.json` is a compiler-consumed index/cache. Future `clyth install`, `clyth remove`, and `clyth refresh` commands should regenerate it from module metadata rather than expecting developers to edit it directly.

### Package-Style Runtime Includes

Runtime packages are imported explicitly by package name:

```clyth
include "collections"
```

This resolves `clyth-runtime/modules/module-collections/module.json`, loads its dependencies, and includes the files listed in its `exports` array. The compiler does not scan for `List<T>`, `Set<T>`, or `Map<K,V>` and inject sources automatically.

Project-local file includes remain file/path oriented. Runtime package includes are package oriented.

### Official Runtime ABI Policy

Official Clyth runtime modules should be implemented in:

```text
Clyth
C
musl-libc-compatible C ABI bindings
```

Official runtime modules should avoid:

```text
C++
libc++
libstdc++
RTTI
C++ exceptions
```

Third-party modules may wrap C++ libraries, but Clyth-facing APIs should expose a C ABI. Official modules are C-lyth, not C++-lyth.

---

## Module Metadata

Each runtime module owns a `module.json` file. Module metadata describes what Clyth source files are exported, what packages are depended on, and which optional C binding artifacts must be built or bundled.

The module format is intentionally plain JSON and should remain inspectable. Future packaged modules should be tarballs containing the same source, metadata, binding artifacts, README, and license files rather than an opaque proprietary package format.

Example shape:

```json
{
  "name": "hash",
  "version": "0.4.0-alpha",
  "kind": "runtime-module",
  "exports": [
    "hash.clyth"
  ],
  "dependencies": [],
  "wrapper": {
    "license": "MIT"
  },
  "bindings": {
    "enabled": false,
    "language": "c",
    "build_system": "zig-cc",
    "build": [
      "zig cc -c ../../c-bindings/rapidhash/rapidhash_wrapper.c -I ../../c-bindings/rapidhash/include -o build/rapidhash_wrapper.o",
      "zig ar rcs build/libclyth_hash_rapidhash.a build/rapidhash_wrapper.o"
    ],
    "outputs": [
      "build/libclyth_hash_rapidhash.a"
    ]
  },
  "c_bindings": [
    {
      "name": "rapidhash",
      "enabled": false,
      "backend": "rapidhash",
      "license": "upstream license metadata required before enabling",
      "source_directory": "../../c-bindings/rapidhash",
      "architectures": {}
    }
  ]
}
```

`bindings.build` is the future `clyth build` / module-build hook. It lets a module describe how to build its C binding without hard-coding that logic into the compiler. The CLI should run those commands, verify the declared outputs exist, and package the generated object/archive files into the module tarball.

For simple C binding shims, official modules should use Zig cc. For existing upstream C projects with established build systems, module metadata may later support CMake/Meson/Make adapters. Official Clyth modules should not require C++ runtime libraries.

---

## DMA Runtime Module

DMA/MMIO is treated as runtime-backed functionality, not as raw pointer syntax.

Clyth code passes integer addresses to explicit runtime functions. The C binding owns the platform-specific implementation details.

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

The public APIs should remain Clyth-owned even when lower-level libraries are used behind the scenes.

---


## Regression Suite Policy

Clyth's language regression suite uses numbered sample files:

```text
NN_0_X_feature_name.clyth
```

`NN` is the stable runner order and `0_X` is the first alpha release line where the feature is supported. New regression tests should be promoted into the numbered suite only after their syntax and expected output reflect current Clyth behavior. Historical/manual examples should not be silently executed by the release runner.

## Building

Build the compiler:

```bash
./build_compiler.sh
```

Fast development build without packaging the distribution:

```bash
./build_compiler.sh --skip-dist
```

Show the compiler version:

```bash
./build-compiler/clyth_compiler_bin --version
```

Compile a sample program:

```bash
./build-compiler/clyth_compiler_bin -c sample-clyth-programs/01_0_1_printf_extern_c.clyth -o printf_test
./printf_test
rm -f printf_test
```

Emit LLVM IR only:

```bash
./build-compiler/clyth_compiler_bin -c sample-clyth-programs/01_0_1_printf_extern_c.clyth -o printf_test --emit-ir-only
```

Preserve generated IR while also linking an executable:

```bash
./build-compiler/clyth_compiler_bin -c sample-clyth-programs/01_0_1_printf_extern_c.clyth -o printf_test --emit-ir
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

The test runner compiles each numbered sample, runs the produced binary, checks expected stdout, prints a visible `[ OK ]` or `[ FAILED ]` result for every sample, and removes generated binaries after the run.

The supported regression suite is numbered by test order and first supported alpha release. See `sample-clyth-programs/REGRESSION_SUITE.md` for the active list. Historical/manual samples live under `sample-clyth-programs/_legacy_untracked/` until they are promoted into the numbered suite.


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


## Documentation Site

The README is intended to remain a concise project introduction and release-facing overview. Longer guides and design references are expected to move into the future GitHub Pages documentation site under `docs/`.

Planned documentation areas include the language guide, runtime module guide, module/package authoring guide, compiler architecture notes, registry/package security documentation, and release notes.

---

## Future CLI Direction

The long-term user-facing command should be `clyth`, with compiler, build, package, module, and registry behavior dispatched by small, explicit subcommands.

Locked command direction:

```bash
# Project creation
clyth new
clyth new-module

# Project lifecycle
clyth build [optional-output-name]
clyth run
clyth test

# Package management
clyth install
clyth remove
clyth search <module-name> [registry-name]
clyth refresh

# Registry management
clyth registry add <name> <location>
clyth registry remove <name>
clyth registry list
```

`clyth build` should build the current project or module. The output name may be supplied explicitly, otherwise it defaults to the project name.

Release-style output is the default. Debuggability is intentionally opt-in:

```bash
clyth build --debug
```

Compiler phase outputs are exposed as build flags rather than separate commands:

```bash
clyth build --emit-tokens
clyth build --emit-parse-tree
clyth build --emit-ast
clyth build --emit-semantics
clyth build --emit-bytecode
clyth build --emit-ir
clyth build --emit-all
```

Registries are first-class. `clyth search json` should search every configured registry, while `clyth search json company` restricts the search to a named registry. Registry locations may be HTTPS endpoints, LAN mirrors, local filesystem directories, USB/offline mirrors, or enterprise repositories. The design is intentionally multi-registry by default rather than assuming a single global package source.

`clyth refresh` updates all configured registry metadata, validates cached metadata/checksums, removes stale entries, and rebuilds the unified search index.

### Package Safety and Install-Time Scanning

`clyth install` should treat package retrieval as a security boundary. Packages may come from official registries, company mirrors, local filesystem repositories, USB/offline mirrors, or community-hosted sources. That flexibility is intentional, but it also means installation must validate what is being imported into a project.

Planned install-time safety checks:

- Verify package checksums against registry metadata and lockfile entries.
- Refuse packages whose metadata does not match the resolved package archive.
- Preserve license and third-party source metadata inside module tarballs.
- Support a configurable scanner hook before a package is installed into the local cache.
- Treat scanner failures as install failures by default.
- Allow organizations to configure their own scanner command for internal registries.

A future configuration may look like this:

```json
{
  "package_security": {
    "scan_on_install": true,
    "scanner": "clamav",
    "command": "clamscan --no-summary {package_archive}",
    "fail_closed": true
  }
}
```

The goal is not to make Clyth responsible for proving that all third-party code is safe. The goal is to make package installation explicit, auditable, checksum-verified, and scanner-friendly by default so users and organizations can enforce their own trust policies.

Internally, compiler, build, package, module, and registry code should remain separate even when invoked through the same user-facing executable.

## MECC Memory Model Direction

MECC means **Managed Entanglement for Collapsible Collections**.

MECC is Clyth's planned deterministic memory-management model. Rather than tracing the whole heap or reference-counting every object individually, MECC groups related allocations into estates and manages lifetime at the estate level.

Current design direction:

- `new Type()` is the normal construction/allocation gateway.
- `estate.new(Type())` explicitly allocates into a specific estate.
- Strong/weak estate relationships are compile-time graph metadata.
- Strong cycles may be rejected or coalesced into shared lifetime groups when safe.
- `malloc`, `free`, and lower-level allocation details are runtime implementation details unless intentionally exposed through low-level modules.

MECC is not fully implemented yet. Alpha 0.4.0 focuses on stabilizing the runtime/module architecture that MECC will eventually use.

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

### Alpha 0.4.0 — Runtime Foundation Release

Release-candidate focus:

- Runtime collections implemented in Clyth source.
- No compiler-owned `List<T>`, `Set<T>`, or `Map<K,V>` backend.
- Explicit package include support such as `include "collections"`.
- `module.json` export loading for bundled runtime packages.
- Numbered regression suite protecting the supported alpha subset.
- Compiler version stamped as `0.4.0-alpha`.
- Official runtime policy: Clyth + C + musl, no C++ runtime dependency for official modules.
- Package/registry CLI direction documented.
- Rapidhash-backed `module-hash` prepared as the next runtime-only improvement. The build/module-build flow should fetch or clone a pinned `Nicoshev/rapidhash` source revision, compile the C wrapper with Zig cc for supported targets, and bundle license metadata with the module artifact.

### Later Releases

Planned areas:

- Thread spawning.
- Thread pools.
- Native JSON parser/writer.
- HTTP/WebSocket runtime.
- TLS runtime.
- Package/module manager with first-class multi-registry search.
- MECC estate runtime.
- Debugger and LSP tooling.
- Benchmark applications.

---

## AI-Assisted Development Philosophy

Clyth is AI-assisted, not AI-authored.

Modern AI-assisted engineering tools have been used during development to accelerate implementation, debugging, refactoring, build-system iteration, and exploration of compiler/runtime design alternatives. That assistance does not define the project. The language design, architecture, roadmap, syntax direction, runtime policy, release decisions, and final code review remain human-directed.

The distinction is intentional. AI is used here as an engineering assistant: useful for producing patch candidates, reviewing failure logs, exploring implementation alternatives, and reducing time spent fighting incidental C++/ANTLR/LLVM complexity. While modern AI tools have accelerated portions of Clyth's implementation, I remain responsible for the language's architecture, long-term direction, release quality, and the technical decisions that shape the project. My goal is to keep the compiler as small as practical while allowing the runtime ecosystem to grow through ordinary Clyth modules, C ABI bindings, and, ultimately, a path toward self-hosting.

This project documents that workflow openly because the tooling used during development should not be confused with authorship, design ownership, or release responsibility.

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

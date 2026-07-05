# Clyth

Clyth is an LLVM-based ahead-of-time systems programming language implemented in C++.

Clyth's design goal is to keep the compiler focused, keep the runtime capable, and keep the language explicit enough for systems work while still giving application developers useful runtime modules.

Alpha 0.5.0 is the **Runtime Ecosystem Foundation Release**. It moves Clyth beyond the "compiler experiment" stage and into a usable alpha language/runtime stack: first-class functions, lambda callbacks, explicit `this`, struct constructor semantics, runtime file I/O, rapidhash-backed collections, yyjson-backed JSON runtime module with singleton-style `JSON` object surface, and HTTP/HTTPS/router foundations built around Clyth-owned APIs.

Documentation: https://mali5820k.github.io/clyth-docs/

---

## Why Clyth Exists

Clyth explores a systems-language design that preserves explicit control and predictable performance while reducing unnecessary ceremony around low-level programming.

The long-term goal is a language capable of writing systems applications, backend services, embedded-facing runtime code, tooling, and eventually its own compiler/runtime ecosystem without forcing ordinary runtime capabilities into compiler internals.

Clyth is designed around these principles:

- Programs should express intent clearly.
- The compiler should own syntax, typing, semantic analysis, and lowering.
- Runtime capabilities should live in Clyth-space modules whenever possible.
- C-backed runtime modules should expose small, auditable C ABIs.
- Modules should become independently buildable and distributable.
- Memory management should eventually support deterministic estate-based ownership through MECC.

---

## Current Status

Clyth currently compiles the Alpha 0.5.0 language subset into native Linux executables through LLVM IR and Zig/musl-based static linking.

Implemented or stabilized areas include:

- ANTLR4 lexer/parser frontend.
- AST generation and semantic analysis infrastructure.
- LLVM IR generation for the supported alpha subset.
- `extern C` declarations and calls.
- `int32 main(string[] args)` command-line argument support.
- Local variables, global variables, user functions, function hoisting, and global hoisting.
- `if`, `else`, `while`, `break`, and `continue`.
- Fixed arrays and compact dynamic arrays.
- Native string values.
- Structs, fields, methods, constructors, explicit `this`, and default struct construction.
- First-class function types using `function<Return, <Args...>>`.
- Lambda/callback syntax such as `function<void, <Request request, Response response> => { ... }`.
- Runtime collections implemented as Clyth modules: `List<T>`, `Set<T>`, and `Map<K,V>`.
- Rapidhash-backed runtime hashing for collections.
- File I/O runtime module with native C binding support.
- HTTP/HTTPS/router API surfaces with Clyth `Request`, `Response`, `Router`, `HttpServer`, and `HttpsServer` values.
- OpenSSL-only TLS direction; wolfSSL is intentionally excluded.
- WebSocket direction through wslay, not the older monolithic WebSocket stack direction.
- JSON runtime module backed by yyjson through a small C ABI binding, exposed through a Clyth `JSON` object with validation, parse, and stringify surface.
- Return-code based regression runner.
- Lean distribution packaging that avoids bundling LLVM source/build trees.

The numbered regression suite currently protects 17 active samples spanning extern C, arrays, strings, structs/methods, generics, package imports, runtime collections, DMA, explicit `this`, first-class functions, file I/O, hashing, HTTP/HTTPS/router API surface, and lambda callbacks.

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

### Runtime Printing

`extern C printf` remains available as a low-level escape hatch, but user-facing printing belongs in `file-io` so Clyth can eventually support formatted strings and object `to_string` behavior without exposing C varargs everywhere.

```clyth
include "file-io"

int32 main() {
    println("Hello from Clyth runtime I/O")
    return 0
}
```

### First-Class Function Types

```clyth
int32 run_callback(function<int32, <int32>> callback) {
    return callback(5)
}

int32 square(int32 value) {
    return value * value
}

int32 main() {
    if run_callback(square) == 25 {
        return 0
    }
    return 1
}
```

### Lambda Callback Syntax

```clyth
include "https"

int32 main() {
    Router router = Router()

    router.post("/", function<void, <Request request, Response response> => {
        if request.body != null {
            print(request.body)
        }
        response.send_json(200, "{\"ok\":true}")
    })

    return 0
}
```

### Collections

```clyth
include "collections"

int32 main() {
    Map<string, int32> scores = Map([
        "one": 1,
        "two": 2
    ])

    if scores.get("two") == 2 {
        return 0
    }
    return 1
}
```

### File I/O

```clyth
include "file-io"

int32 main() {
    string path = "example.txt"
    create_file(path)
    write_text(path, "hello")
    append_line(path, " from Clyth")

    if file_exists(path) {
        delete_file(path)
        return 0
    }
    return 1
}
```

### HTTPS/Router Direction

```clyth
include "file-io"
include "https"
include "json"

int32 main(string[] args) {
    string key = file.read("path-to-key.pem")
    string cert = file.read("path-to-cert.pem")

    Router router = Router()

    router.post("/", function<void, <Request request, Response response> => {
        response.send_json(200, "{\"ok\":true}")
    })

    HttpsConfiguration config = HttpsConfiguration(key, cert, 9000, router)
    HttpsServer server = HttpsServer(config)

    return 0
}
```

---

## Runtime Modules

Clyth runtime functionality is organized under `clyth-runtime/`.

```text
clyth-runtime/
├── c-bindings/
│   ├── dma/
│   ├── file-io/
│   ├── http/
│   ├── openssl/
│   └── rapidhash/
├── modules/
│   ├── module-collections/
│   ├── module-dma/
│   ├── module-file-io/
│   ├── module-hash/
│   ├── module-http/
│   ├── module-https/
│   ├── module-json/
│   └── module-router/
└── runtime_libraries.json
```

The Clyth-owned HTTP/WebSocket/backend stack direction is:

```text
libuv    -> event loop, async file I/O, TCP, timers, worker scheduling
llhttp   -> HTTP/1.1 parser
OpenSSL  -> TLS/HTTPS
wslay    -> WebSocket framing after HTTP Upgrade
```

Clyth no longer targets wslay for the official runtime stack.

---

## JSON Status

JSON is an official runtime module in 0.5.0. JSON remains runtime work, not compiler work, and is exposed through a Clyth-owned `JSON` object surface backed by yyjson through a small C ABI binding.

Current support:

- HTTP/HTTPS APIs can send JSON payload strings.
- `module-json` establishes `JsonValue` and early JSON value helpers.
- JSON is documented as a Clyth-owned runtime feature built from strings, arrays, maps, keyed arrays, and struct formatting.

Near-term support:

- `JSON.stringify(value)` for structs, arrays, maps, and primitives.
- `JSON.parse(string)` returning a `JsonValue` tree.
- Object/array builders.
- `to_string()` / formatted string integration.

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

Run regression samples:

```bash
./build_compiler.sh --skip-dist
./sample-clyth-programs/run_language_samples.sh
rm -rf sample-clyth-programs/.sample-bin
```

---

## Regression Suite Policy

Clyth's language regression suite uses numbered sample files:

```text
NN_0_X_feature_name.clyth
```

The runner compiles every numbered sample, executes the produced binary, and treats exit code `0` as pass. Any non-zero exit code is failure. Tests should validate behavior internally rather than depending on fragile stdout comparisons.

---

## Distribution

The build script can create:

```text
dist-clyth/
clyth-dist.tar.gz
```

The distribution should remain lean: compiler binary, runtime files, sample programs, useful inspection tools, and license files. LLVM source trees, LLVM build directories, Zig toolchains, generated object caches, and vendor source checkouts should not be copied into the distribution.

---

## MECC Memory Model Direction

MECC means **Managed Entanglement for Collapsible Collections**.

MECC is Clyth's planned deterministic memory-management model. Rather than tracing the whole heap or reference-counting every object individually, MECC groups related allocations into estates and manages lifetime at the estate level.

0.5.0 intentionally strengthens the runtime/module ecosystem before MECC so future ownership analysis has real application patterns to analyze: files, requests, responses, collections, async tasks, and runtime module boundaries.

---

## AI-Assisted Development Philosophy

Clyth is AI-assisted, not AI-authored.

Modern AI-assisted engineering tools have been used during development to accelerate implementation, debugging, refactoring, build-system iteration, and exploration of compiler/runtime design alternatives. That assistance does not define the project. The language design, architecture, roadmap, syntax direction, runtime policy, release decisions, and final code review remain human-directed.

This project documents that workflow openly because tooling used during development should not be confused with authorship, design ownership, or release responsibility.

---

## Legal Notes

Clyth prefers permissive licensing for bundled runtime pieces and external dependencies.

Preferred external dependency licenses:

- MIT
- BSD-2-Clause
- BSD-3-Clause
- Apache-2.0 when appropriate and explicitly tracked

Avoided unless absolutely necessary:

- GPL
- LGPL
- AGPL
- other licenses that complicate static distribution

Third-party project names are used to describe architecture and dependency relationships. Clyth is not affiliated with, endorsed by, or sponsored by those projects or their maintainers.


## 0.5.0 JSON Runtime Note

Clyth 0.5.0 treats JSON as runtime functionality, not compiler functionality. The official JSON module exposes a Clyth-owned `JSON` object surface backed by a thin C ABI shim over yyjson. libuv remains the event-loop / async I/O foundation; it is not responsible for JSON parsing.

The first 0.5.0 JSON layer validates, classifies, and carries JSON payloads through `JsonValue`. Follow-up hardening expands this into owned parse trees, object/array builders, and stringify support for primitives, maps, arrays, structs, and HTTP/HTTPS response payloads.

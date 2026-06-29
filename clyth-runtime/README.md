# Clyth Runtime

The Clyth runtime is organized around installable/runtime modules. Runtime module source lives under `clyth-runtime/modules/`, while C binding source code lives under `clyth-runtime/c-bindings/`.

`runtime_libraries.json` is a compiler-consumed index/cache generated from module metadata. The source of truth for each module is its `module.json`. Future commands such as `clyth install`, `clyth uninstall`, and `clyth rebuild-index` should update this index automatically.

## Layout

```text
clyth-runtime/
├── c-bindings/          # C source stubs used to build static binding archives
├── modules/             # Clyth runtime modules and packaged module metadata
├── runtime_libraries.json
└── EXTERNAL_LIBRARIES_LICENSES.md
```

## Module Metadata

Each module has a `module.json` file. Clyth wrapper code and native C bindings are licensed separately so generated program license bundles can be precise.

```json
{
  "name": "dma",
  "version": "0.3.0-alpha",
  "kind": "runtime-module",
  "exports": ["dma.clyth"],
  "wrapper": { "license": "MIT" },
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

## Boundary

Compiler-backed primitives: arrays, strings, structs, functions, generic syntax, keyed-array literals, and lowering. Runtime-owned features: List, Set, Map, JSON, web/TLS wrappers, DMA APIs, estates, and future allocation policies.

Arrays are compact primitives with `data` and `length`. Growable capacity belongs to runtime containers such as `List<T>`, `Set<T>`, and `Map<K,V>`.

## Alpha 0.4 Pass 2 Bridge

Runtime modules are imported explicitly. Collection types become visible through `include "collections"`; the compiler no longer injects List/Set/Map sources by scanning for container names. The old scaffold-backed List/Set/Map lowering path has been removed from the active backend; runtime collections lower through ordinary generic structs, constructors, methods, compact arrays, and keyed-array literals.

Explicit stack-style construction is also accepted during the migration:

```clyth
include "collections"

List<int32> values = List([1, 2, 3])
Set<int32> unique = Set([1, 2, 2])
Map<int32, int32> table = Map([1: 10, 2: 20])
```

This is normalized onto the existing scaffold-backed lowering until full monomorphization replaces it in the next passes.

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

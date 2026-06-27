# Clyth Runtime Module: collections

License: MIT

This module owns Clyth-space runtime collection definitions.

Alpha 0.4 migration direction:

- `List<T>` owns growable array behavior, capacity, and head/tail indexing.
- `Set<T>` builds on `List<T>` first, then moves to hash-backed storage through `module-hash`.
- `Map<K,V>` uses bucket chaining through `List<List<MapEntry<K,V>>>` and delegates hashing to `module-hash` once the rapidhash backend is enabled.

The compiler should eventually only provide arrays, generic instantiation, constructors, method dispatch, and keyed-array lowering. Collection behavior should live here, not in compiler-specific lowering.

## Current migration status

The runtime source now expresses the intended List/Set/Map behavior, including List removal helpers and bucket-chained Map removal logic.

Alpha 0.4 pass 5A adds generic-instantiation discovery in the compiler pipeline: concrete uses such as `List<int32>` and `Map<int32,int32>` are detected, assigned stable concrete names such as `List__int32`, and preview-substituted before generic templates are stripped from the current backend. Compiler scaffolding is still active until later 5B/5C/5D passes can emit and execute those instantiated runtime definitions directly.


## Alpha 0.4 pass 5G note

During the migration, collection module source files may be resolved and tracked by the compiler while their generic template bodies are deferred from the current parse stream. This prevents nested generic runtime templates from being parsed before executable monomorphized runtime structs are ready. The temporary compiler scaffold remains active until the runtime generic compile path fully replaces it.

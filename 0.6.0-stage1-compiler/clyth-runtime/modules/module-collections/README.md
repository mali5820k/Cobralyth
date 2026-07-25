# Clyth Runtime Module: collections

License: MIT

This module owns Clyth-space runtime collection definitions. It is imported explicitly with:

```clyth
include "collections"
```

Exports:

- `List<T>`
- `Set<T>`
- `Map<K,V>`

Alpha 0.4.0 status:

- Collections are runtime modules, not compiler-owned backends.
- `List<T>` is implemented in Clyth using compiler-backed arrays, structs, constructors, methods, and generic instantiation.
- `Set<T>` is currently a primitive array-backed Clyth implementation with duplicate prevention.
- `Map<K,V>` is currently a primitive `MapEntry<K,V>[]`-backed Clyth implementation with keyed-array constructor support and linear lookup.
- Future hash-backed `Map`/`Set` storage should use `module-hash`; the compiler must not regain collection-specific lowering.

Compiler-owned primitives used by this module:

```text
arrays
keyed-array literals
structs
generic structs
constructors
methods
implicit this lookup
function/method calls
```

Do not add compiler branches for `List`, `Set`, or `Map`. If a collection behavior is missing, improve the language primitive or implement the behavior in Clyth runtime source.

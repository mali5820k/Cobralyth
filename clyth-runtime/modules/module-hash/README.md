# Clyth Runtime Module: hash

This module owns hashing APIs for runtime containers.

`Map<K,V>` and `Set<T>` should eventually depend on this module instead of relying on compiler-owned hashing or comparison behavior.

The planned C backend is rapidhash. The backend remains disabled until its vendored source and license metadata are added to `clyth-runtime/c-bindings/rapidhash/`.

# rapidhash C Binding

This directory is reserved for the rapidhash C binding used by `module-hash`.

The binding should remain a C ABI shim compiled with Zig cc. Do not introduce C++ runtime dependencies.

Planned layout:

```text
rapidhash/
  README.md
  LICENSE.md
  include/
    rapidhash.h
    clyth_rapidhash.h
  src/
    clyth_rapidhash.c
  third_party/
    rapidhash/
```

The upstream rapidhash source should be pinned to an exact commit or tag before enabling the binding in `module-hash/module.json`.

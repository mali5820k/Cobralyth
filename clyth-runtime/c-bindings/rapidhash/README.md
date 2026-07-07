# Clyth rapidhash binding

This C binding builds a thin Clyth ABI layer over the upstream `Nicoshev/rapidhash` v4 source.

Build policy:

- `build_compiler.sh` clones `https://github.com/Nicoshev/rapidhash.git` into `vendor/rapidhash` when the vendor checkout is missing.
- The Clyth repository should not commit generated `.o` or `.a` archives.
- Built archives are emitted under `c-bindings/rapidhash/<arch>/` and copied into `modules/module-hash/<arch>/`.
- The module exposed to Clyth code remains `include "hash"`; the implementation backend is rapidhash.

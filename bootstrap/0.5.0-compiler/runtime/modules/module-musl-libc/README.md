# musl-libc module

`musl-libc` is an ordinary Clyth module whose manifest declares
`"linkStrategy": "compiler_core"`. The compiler therefore includes it in
normal builds for compatible Linux targets without exposing libc symbols in a
source namespace.

Other Clyth modules may declare the exact `extern C` symbols they need and wrap
them with Clyth-native interfaces. The module owns the precompiled target
artifacts, target compatibility metadata, pinned upstream version, and exact
upstream attribution material.

The canonical upstream `VERSION` and `COPYRIGHT` files are copied from the
pinned musl source tree by `scripts/build_musl.sh`. The `COPYRIGHT` file is both
the complete license notice and musl's upstream-maintained contributor list.

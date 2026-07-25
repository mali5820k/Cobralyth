# Clyth Runtime Module: hash

License: MIT wrapper

This module owns hashing APIs for runtime containers and other Clyth libraries.

Alpha 0.4.0 status:

- The module currently exposes safe Clyth-only stub functions so dependent modules can compile.
- The planned backend is rapidhash through a C ABI wrapper.
- The rapidhash backend remains disabled until the exact pinned source revision and license metadata are present.

Planned build flow:

1. Fetch or clone a pinned `Nicoshev/rapidhash` revision during `clyth build` / module-build.
2. Compile the C wrapper with Zig cc.
3. Archive the binding output into a static library.
4. Package the Clyth wrapper, archive/object files, module metadata, and license files into an inspectable module tarball.

Official runtime policy: Clyth + C + musl-compatible ABI only. No official C++ runtime dependency.

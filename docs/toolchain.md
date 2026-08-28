# Clyth native toolchain

The bootstrap compiler keeps the code generator, native wrapper compiler, and C runtime provider separate.

- `llc` lowers Clyth-generated LLVM IR to native objects.
- `lld`/`ld.lld` links objects and static module archives.
- Zig compiles native C/C++ module wrappers and supplies target compiler-runtime builtins when an architecture requires them.
- The Clyth-managed musl sysroot always supplies libc headers, CRT objects, and `libc.a`. Zig's bundled libc is not the Clyth runtime ABI.

Resolution may be supplied by command-line flags or environment variables:

```text
--target / Clyth target
--zig / CLYTH_ZIG
--llc / CLYTH_LLC
--lld / CLYTH_LLD
--musl-sysroot / CLYTH_MUSL_SYSROOT
```

For repository-local source builds, quoted imports are compiled into separate objects and linked together. Unquoted runtime-module imports with native bindings are compiled through Zig against the selected Clyth musl sysroot. Vendor-backed module builds remain package-manager/module-build responsibilities until pinned vendor fetching is moved into the Go package manager.

AArch64 musl may reference compiler-runtime helpers such as quad-precision arithmetic builtins. When required, the bootstrap compiler uses the selected Zig installation to build Zig's `compiler_rt.zig` for the target and links that archive alongside Clyth musl. This does not make Zig's libc the runtime provider.

## Musl source and sysroots

Clyth pins musl 1.2.6. The normal full build is intentionally one-command: `scripts/build-all.sh` prepares the default host musl sysroot before native modules or the self-host compiler are built.

Preparation follows this order:

1. Reuse a valid `CLYTH_MUSL_SYSROOT` supplied by the caller.
2. Otherwise reuse `precompiled/<target>` when it already contains the Clyth-managed headers, CRT objects, and `libc.a`.
3. Otherwise use the existing verified source tree at `third_party/musl`.
4. If that source tree is absent, run `scripts/fetch_musl.sh` to download and SHA-256 verify the pinned 1.2.6 archive.
5. Build the required sysroot with `scripts/build_musl.sh <target>`.

The smaller musl scripts remain useful for explicit multi-target preparation, for example `scripts/build_musl.sh aarch64-linux-musl`. `scripts/check-backend.sh` accepts `CLYTH_MUSL_X86_64_SYSROOT` and `CLYTH_MUSL_AARCH64_SYSROOT` when validating native links for both architectures.

## Vendor source cache

Pinned third-party source required by native runtime bindings is stored under `module-src/vendor/<name>/<commit>/`. The cache is source input rather than a build artifact and therefore survives ordinary `clean.sh` and `build-all.sh --clean` runs. `clean.sh --vendors` explicitly removes it when a fresh fetch is desired.

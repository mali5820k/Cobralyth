# Clyth 0.4.0-alpha — Runtime Foundation Release

Clyth 0.4.0-alpha stabilizes the compiler/runtime boundary and turns the runtime collections into ordinary Clyth modules.

## Highlights

- Runtime `List<T>`, `Set<T>`, and `Map<K,V>` now live in the `collections` runtime package.
- Collection usage is explicit through `include "collections"`.
- Compiler-owned collection backends and implicit collection injection were removed.
- Generic structs, constructors, methods, implicit `this`, arrays, strings, keyed-array literals, and runtime package includes are covered by the numbered regression suite.
- Compiler version reporting now identifies this release as `0.4.0-alpha`.
- Documentation now reflects the 0.4.0 architecture: a smaller compiler, richer Clyth runtime modules, C ABI bindings, and a path toward self-hosting.

## Verification

The active 0.4.0 language regression suite contains 11 numbered tests and currently passes 11/11.

```bash
./build_compiler.sh --skip-dist
./build-compiler/clyth_compiler_bin --version
./sample-clyth-programs/run_language_samples.sh
rm -rf sample-clyth-programs/.sample-bin
```

## Deferred to 0.5.0+

- Rapidhash-backed `module-hash`
- `clyth build` project/module orchestration
- `clyth new-module` scaffolding
- package registry/search/install implementation
- first-class functions and broader language ergonomics

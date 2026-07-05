# Clyth Language Regression Suite

This directory contains the supported language regression suite for Clyth alpha releases.

File naming convention:

```text
NN_0_X_feature_name.clyth
```

- `NN` is the stable test order used by `run_language_samples.sh`.
- `0_X` is the first alpha release line where the behavior is considered supported.
- The suffix names the feature or regression being protected.

## Active 0.5.0 regression set

| Test | First supported | File | Protects |
|---:|---|---|---|
| 01 | 0.1 | `01_0_1_printf_extern_c.clyth` | Minimal executable generation and extern-C calls. |
| 02 | 0.2 | `02_0_2_dynamic_array.clyth` | Native dynamic arrays, indexing, length/capacity access. |
| 03 | 0.2 | `03_0_2_native_string.clyth` | Native string creation, assignment, and length/capacity access. |
| 04 | 0.4 | `04_0_4_runtime_list_collection.clyth` | Runtime `List<T>` implemented as Clyth source via `include "collections"`. |
| 05 | 0.4 | `05_0_4_runtime_set_collection.clyth` | Runtime `Set<T>` implemented as Clyth source via `include "collections"`. |
| 06 | 0.4 | `06_0_4_runtime_map_collection.clyth` | Runtime `Map<K,V>` implemented as Clyth source via `include "collections"`. |
| 07 | 0.2 | `07_0_2_struct_methods_release.clyth` | Struct field access, method dispatch, and method calls. |
| 08 | 0.4 | `08_0_4_runtime_dma_include.clyth` | Runtime module include plumbing. |
| 09 | 0.4 | `09_0_4_generic_runtime_package_include.clyth` | Package-style `include "collections"` and generic runtime templates. |
| 10 | 0.4 | `10_0_4_runtime_collection_constructors.clyth` | Constructor-based runtime collections and keyed-array constructor input. |
| 11 | 0.4 | `11_0_4_generic_user_templates.clyth` | User-authored generic structs and instantiated generic values. |
| 12 | 0.5 | `12_0_5_explicit_this_methods.clyth` | Explicit `this.field` reads and assignments inside methods/constructors. |

## Legacy/untracked samples

Files in `_legacy_untracked/` are preserved only as historical/manual reference material.
They are not part of the supported release regression suite until they are renamed,
assigned a release milestone, given an expected output, and added to `run_language_samples.sh`.

- `13_0_5_first_class_function_callback.clyth` — validates first-class function callback syntax and indirect calls through `function<return<params>>`.

- `14_0_5_file_io_runtime.clyth` — validates the native file runtime module can write, append, stat, and remove a text file.

- `15_0_5_hash_runtime.clyth` — validates the runtime hash module and rapidhash replacement ABI are linked and callable.

- `16_0_5_https_api_surface.clyth` — validates the HTTPS module API surface links without enabling a TLS backend yet.

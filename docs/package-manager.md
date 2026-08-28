# Clyth V1.0.0 Package Manager Contract

`clyth_project.json` is the human-authored project manifest. It records `project-name`, `version`, `project-type` (`program` or `module`), the SPDX license identifier and license-file path, the entry source, and direct dependency constraints.

`clyth_package.json` is machine-generated resolution state. It records exact direct and transitive versions, registry identity, checksums, current artifact/source locations, installation provenance, and upstream license metadata. Normal builds consume this locked graph rather than silently resolving newer versions.

## Commands

- `clyth install <module> [--build-from-source] [--from-registry=name]`
- `clyth install -f <module-tarball>`
- `clyth update [module] [--build-from-source]`
- `clyth remove <module>`
- `clyth list`
- `clyth info <module>`
- `clyth refresh`
- `clyth init <project-name>`
- `clyth add-registry <name> (--path <dir> | --url <url>)`
- `clyth remove-registry <name>`
- `clyth list-registries`
- `clyth registry-info <name>`
- `clyth debug`

`refresh` is transactional. Every exact locked registry package must still exist in its recorded registry and match its locked checksum before any package URL/path is rewritten. It never changes package versions or registry identity.

`--build-from-source` preserves package identity and exact source revision. It is intended for cases where a registry package has source compatible with the local Clyth compiler but no compatible prebuilt artifact for the local compiler/musl toolchain. The current Go bootstrap implements the pinned source-fetch/provenance path; native source compilation becomes active when LLVM lowering is completed.

A final program distributable has the form:

```text
<project-name>-dist/
  <project-name>
  <project-license-file>
  THIRD_PARTY_LICENSES.md
```

Module distributions include the same licensing material in the module archive. `THIRD_PARTY_LICENSES.md` is assembled from the exact transitive dependency graph and preserves upstream license text/metadata.

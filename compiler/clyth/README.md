# Clyth self-hosted compiler

This directory contains the compiler implementation written in Clyth.
It is compiled by the Go bootstrap/reference compiler during the bootstrap phase.

Build from the repository root with:

```sh
./scripts/build-bootstrap.sh
./compiler/clyth/build.sh
```

Regression samples live at the repository root in `sample-clyth-programs/` so the
same corpus can be run against either compiler implementation.

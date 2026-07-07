# Clyth yyjson Binding

This directory contains Clyth's thin C ABI shim for the official JSON runtime module.

The upstream JSON parser/writer is vendored at build time from `ibireme/yyjson` and is not committed as generated source. The Clyth runtime exposes a Clyth-owned JSON module surface while yyjson performs the low-level parse/validate/write work.

Clyth uses the project name only to describe runtime architecture and dependency composition. Clyth is not affiliated with, endorsed by, or sponsored by yyjson or its maintainers.

# Clyth libwebsockets Binding License

## Clyth binding code

The Clyth wrapper/source files created in this repository for libwebsockets are licensed under the MIT License as part of the Clyth runtime.

## Backing library

The backing library intended for this binding is libwebsockets.

- Upstream project: https://github.com/warmcat/libwebsockets
- Upstream license: MIT, with some bundled permissive components retained under compatible BSD/Zlib/CC0-style terms as noted by the upstream project.

When a compiled libwebsockets archive is bundled into a Clyth Module, the exact upstream libwebsockets license text and any retained third-party notices must be copied into that module's `license.md` and into the generated program license bundle.

This directory currently contains Clyth runtime binding scaffolding only; it does not vendor libwebsockets source code.

# clyth-lsp

`clyth-lsp` is Clyth's editor-independent Language Server Protocol process. It communicates over stdio and is intentionally a separate Go binary from both compiler implementations.

Current bootstrap features: full-document synchronization, delimiter/string/comment diagnostics, completion for Clyth keywords/types and open-document symbols, hover, document symbols, go-to-definition, references, and rename across open Clyth documents.

This server intentionally uses a lightweight document index until the Go compiler frontend is stable. The long-term design is for the compiler and LSP to share the same lexer, Pratt parser, AST, semantic model, and diagnostics packages so editor behavior cannot drift from the compiler.

Build:

```sh
go build -o ../../../dist/bin/clyth-lsp ./cmd/clyth-lsp
```

The editor packages under `../editors/` expect `clyth-lsp` to be available on `PATH` unless their local README says otherwise.

The server also publishes semantic tokens for callable type declarations, module namespaces, quoted source-file imports, user-defined types/type parameters, constructors, and ordinary function calls.

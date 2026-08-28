# Go -> Clyth bootstrap transpilation manifest

Frozen Go implementation sources were translated in one structural blitz before compile repair.

| Go source | Clyth seed | Go lines | Clyth lines |
|---|---|---:|---:|
| `cmd/clyth/main.go` | `compiler/clyth/bootstrap-v1/cmd/clyth/main.clyth` | 72 | 74 |
| `internal/compiler/build.go` | `compiler/clyth/bootstrap-v1/internal/compiler/build.clyth` | 486 | 488 |
| `internal/compiler/frontend.go` | `compiler/clyth/bootstrap-v1/internal/compiler/frontend.clyth` | 44 | 46 |
| `internal/compiler/lifetime.go` | `compiler/clyth/bootstrap-v1/internal/compiler/lifetime.clyth` | 992 | 994 |
| `internal/compiler/llvm_lowering.go` | `compiler/clyth/bootstrap-v1/internal/compiler/llvm_lowering.clyth` | 3224 | 3226 |
| `internal/compiler/semantic_analyzer.go` | `compiler/clyth/bootstrap-v1/internal/compiler/semantic_analyzer.clyth` | 1747 | 1749 |
| `internal/compiler/semantic_types.go` | `compiler/clyth/bootstrap-v1/internal/compiler/semantic_types.clyth` | 535 | 532 |
| `internal/compiler/toolchain.go` | `compiler/clyth/bootstrap-v1/internal/compiler/toolchain.clyth` | 552 | 555 |
| `internal/frontend/ast/ast.go` | `compiler/clyth/bootstrap-v1/internal/frontend/ast/ast.clyth` | 481 | 487 |
| `internal/frontend/lexer/lexer.go` | `compiler/clyth/bootstrap-v1/internal/frontend/lexer/lexer.clyth` | 293 | 295 |
| `internal/frontend/lexer/trie.go` | `compiler/clyth/bootstrap-v1/internal/frontend/lexer/trie.clyth` | 57 | 61 |
| `internal/frontend/parser/parser.go` | `compiler/clyth/bootstrap-v1/internal/frontend/parser/parser.clyth` | 1231 | 1232 |
| `internal/frontend/token/token.go` | `compiler/clyth/bootstrap-v1/internal/frontend/token/token.clyth` | 125 | 128 |
| `internal/packagecmd/package.go` | `compiler/clyth/bootstrap-v1/internal/packagecmd/package.clyth` | 856 | 859 |
| `internal/packagecmd/project.go` | `compiler/clyth/bootstrap-v1/internal/packagecmd/project.clyth` | 140 | 143 |
| `internal/version/version.go` | `compiler/clyth/bootstrap-v1/internal/version/version.clyth` | 4 | 8 |

# Clyth editor and LSP support

Clyth uses one Go `clyth-lsp` implementation across supported editors. Editor-specific integration lives under `lsp/editors/`; release-ready packages are generated under `lsp/dist/`.

```text
lsp/
├── server/                  # Go stdio LSP server
├── editors/                 # integration source
│   ├── vscode/
│   ├── notepadpp/
│   ├── vim/
│   ├── neovim/
│   └── fresh/
└── dist/                    # generated installable artifacts
    ├── fresh-clyth-lsp.tar.gz
    ├── vscode-clyth-lsp.VSIX
    ├── vim-clyth-lsp.tar.gz
    └── nvim-clyth-lsp.tar.gz
```

Build all editor packages with:

```sh
./scripts/package-lsp.sh
```

The VS Code VSIX is self-contained: it includes the extension code and prebuilt `clyth-lsp` binaries and does **not** require an end user to run `npm install`, install Node packages, or separately configure the server path.

The Fresh/Vim/Neovim archives contain their editor integration, prebuilt Linux/macOS LSP binaries, and an `install.sh` helper. Extract the archive and run `./install.sh`.

Notepad++ remains source-configured under `lsp/editors/notepadpp/` because Notepad++ requires an external LSP-client plugin; its UDL and NppLspClient configuration remain available there.

## Architectural direction

The Go language server currently owns a lightweight document index and structural diagnostics. As the Go bootstrap compiler frontend stabilizes, its lexer, Pratt parser, AST, semantic model, and diagnostics should become shared packages consumed by both the compiler and `clyth-lsp`; the LSP should not grow an independent compiler frontend.

## Licensing

Clyth LSP/editor support is distributed under the repository's MIT license. A copy of `LICENSE` is kept in the LSP root, language-server source directory, and each editor integration source directory. Generated editor artifacts also contain the MIT license and integration-specific `THIRD_PARTY_NOTICES.md` files.

The Go language server itself has no third-party Go module dependencies. Vim and Notepad++ integrations rely on separately installed MIT-licensed LSP client plugins; see `THIRD_PARTY_NOTICES.md` for details.

## Highlighting model

Editor grammars provide immediate lexical highlighting for Clyth declarations, imports (including hyphenated package names such as `file-io`), built-in types, and conventional capitalized user-defined types/type parameters. The LSP remains the long-term semantic source of truth; these lexical rules intentionally keep editor support useful before the shared Go compiler semantic frontend is complete.

## Syntax-aware highlighting

The editor grammars and LSP recognize the current Clyth syntax distinctions:

```clyth
import file-io                  // module/package namespace
import "support/helper.clyth"  // direct source-file path

void() callback
int32(string) parser
Response(Request, Response) handler
bool(Request, Response)[] filters
```

`clyth-lsp` advertises LSP semantic tokens for namespaces, types, type parameters, functions, callable variables, and quoted source paths. This lets semantic-token-capable editors distinguish callable type declarations from ordinary function/constructor calls even when their punctuation is superficially similar. VS Code's packaged extension consumes these tokens directly; other LSP clients may use them when semantic-token support is enabled.

Explicit casts use `cast<T>(value)` and therefore remain syntactically separate from callable types.

## V1.0.0 lambda syntax

Editor grammars and the language server recognize the canonical `ReturnType(typed parameters) => { ... }` lambda form, contextually typed `(parameters) => { ... }` lambdas, `auto` inference from self-typed lambdas, scoped `enum`, `switch`, and `cast<T>(expr)`.

Struct members are public by default. `private` members are visible only from methods, constructors, and destructors of the declaring struct; `public` is accepted as an explicit default.

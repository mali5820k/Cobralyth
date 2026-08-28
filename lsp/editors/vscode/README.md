# VS Code

Install the prebuilt `lsp/dist/vscode-clyth-lsp.VSIX` from **Extensions → … → Install from VSIX...** or with:

```sh
code --install-extension vscode-clyth-lsp.VSIX
```

The VSIX contains the Clyth syntax grammar and prebuilt `clyth-lsp` servers for Linux, macOS, and Windows on the supported architectures. No `npm install` or separate LSP installation is required.

`clyth.lsp.path` remains available as an optional override for development builds.

The extension consumes `clyth-lsp` semantic tokens so callable type declarations such as `bool(Request, Response)[] filters` are visually distinct from calls and constructors. Unquoted module imports and quoted source-file imports are also tokenized separately.

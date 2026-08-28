# Clyth LSP Third-Party Notices

The Clyth language server (`clyth-lsp`) uses only the Go standard library and does not bundle third-party Go modules.

Editor integrations are Clyth-authored. Some editors require external host/plugin components which are **not bundled** with Clyth:

- Vim: `vim-lsp` (MIT), https://github.com/prabirshrestha/vim-lsp
- Notepad++: `NppLspClient` (MIT), https://github.com/Ekopalypse/NppLspClient

VS Code uses host-provided VS Code/Node APIs and bundles no npm packages. Neovim uses its built-in LSP client. Fresh uses its native language-pack/LSP integration.

Each editor source directory and each generated distributable includes the Clyth MIT `LICENSE` and an integration-specific `THIRD_PARTY_NOTICES.md`.

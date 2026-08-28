# Notepad++

Notepad++ itself does not provide a native LSP client. Clyth support is therefore split into two pieces:

1. Import `Clyth.xml` using **Language -> User Defined Language -> Define your language -> Import**. This provides `.clyth` identification and highlighting.
2. Install the third-party **NppLspClient** plugin, open a Clyth project with **Folder as Workspace**, and merge `NppLspConfig.toml` into its configuration. Ensure `clyth-lsp.exe` is on `PATH` or replace the executable with an absolute path.

The NppLspClient plugin currently uses the active Notepad++ language name to select its `[lspservers.<language>]` section, which is why the section is named `Clyth` to match the UDL.

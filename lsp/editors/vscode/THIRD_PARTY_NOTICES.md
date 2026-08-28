# Third-Party Notices

This Clyth VS Code extension bundles Clyth-authored editor integration files and prebuilt `clyth-lsp` binaries only.

It does not bundle npm packages or `vscode-languageclient`. The extension uses APIs provided by the VS Code host and Node.js runtime (`vscode`, `child_process`, and `path`) at execution time; those host components are not redistributed in this package.

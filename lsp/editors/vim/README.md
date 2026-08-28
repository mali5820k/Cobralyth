# Vim

Copy `ftdetect/`, `syntax/`, and optionally `plugin/` into a directory on Vim's `runtimepath` (for example `~/.vim/`). Syntax highlighting works without plugins.

For LSP features, the supplied plugin file auto-registers `clyth-lsp` when `prabirshrestha/vim-lsp` is installed and the executable is on `PATH`. Other Vim LSP clients can simply launch `clyth-lsp` over stdio for the `clyth` filetype.

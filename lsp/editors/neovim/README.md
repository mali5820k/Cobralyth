# Neovim 0.11+

Copy `ftdetect/clyth.vim` and `lsp/clyth.lua` into your Neovim config directory (normally `~/.config/nvim/`), ensure `clyth-lsp` is on `PATH`, and add:

```lua
vim.lsp.enable('clyth')
```

Neovim 0.11+ discovers `lsp/clyth.lua` through the built-in LSP configuration system; no `require('lspconfig')` setup is required.

For syntax highlighting until a Clyth tree-sitter grammar exists, you may also copy `../vim/syntax/clyth.vim` to `~/.config/nvim/syntax/clyth.vim`.

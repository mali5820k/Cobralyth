" Optional automatic registration for prabirshrestha/vim-lsp.
if exists('*lsp#register_server') && executable('clyth-lsp')
  augroup clyth_lsp_registration
    autocmd!
    autocmd User lsp_setup call lsp#register_server({
          \ 'name': 'clyth-lsp',
          \ 'cmd': {server_info->['clyth-lsp']},
          \ 'allowlist': ['clyth'],
          \ })
  augroup END
endif

au FileType cpp nnoremap <buffer> <F10> :setlocal makeprg&<CR>:wa<CR>:make! KPR_REAL_TIME_UPDATE=1<CR><C-l>:cw <Bar> :cc<CR>
au BufWinEnter */tests/*.cpp nnoremap <buffer> <F10> :setlocal makeprg&<CR>:wa<CR>:make! test<CR><C-l>:cw <Bar> :cc<CR>

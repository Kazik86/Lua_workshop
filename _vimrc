au BufRead ./tests/*.cpp nnoremap <buffer> <F10> :setlocal makeprg&<CR>:wa<CR>:make! test<CR><C-l>:cw <Bar> :cc<CR>

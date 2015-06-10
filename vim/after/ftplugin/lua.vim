fun! s:Get_Char_Under_Cursor()
    return getline(".")[col(".") - 1]
endf

fun! s:Calc_Range()
    let orig_pos = getpos(".")
    call cursor(line("."), 1)
    let block_beg = search('^[^e}[:space:]]', "bcnW")
    let block_end = search('^[e}]', "cnW")
    call setpos(".", orig_pos)
    if block_beg != 0 && block_end != 0
	return [block_beg, block_end]
    endif
    return [0, 0]
endf

fun! s:Select_Range(beg, end)
    call cursor(a:beg, 1)
    normal! V
    call cursor(a:end, 1)
endf

fun! s:Select_State_Or_Fun()
    let [beg_line, end_line] = <SID>Calc_Range()
    if beg_line != 0 && end_line != 0
	call <SID>Select_Range(beg_line, end_line)
	return 1
    endif
    return 0
endf

let s:rtu_tempfile = ""

fun! s:Write_To_Temp_File(text)
    if s:rtu_tempfile == ""
	let s:rtu_tempfile = tempname()
    endif

    exe 'redir! >' s:rtu_tempfile
    silent echo a:text
    redir END
    return s:rtu_tempfile
endf

fun! s:Send_To_RTU_Session()
    let c_orig = col('.')
    let l_orig = line('.')

    if <SID>Select_State_Or_Fun()
	let reg = @r
	keepjumps normal! "ry
	let file = s:Write_To_Temp_File(@r)
	let @r = reg
	normal! gv
	redraw
	silent exe '!echo -n ' . shellescape(expand("%") . ";" . file) . ' | socat - UNIX-SENDTO:/tmp/rtu'
	normal! 
	call cursor(l_orig, c_orig)
    endif
endf

nnoremap <buffer> <A-p> :call <SID>Send_To_RTU_Session()<CR>

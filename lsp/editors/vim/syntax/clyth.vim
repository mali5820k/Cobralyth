if exists("b:current_syntax") | finish | endif
syn keyword clythControl if else for while switch case default break continue return as
syn keyword clythDecl auto enum struct protocol module import extern new const cast private public C this
syn keyword clythType void bool char string int8 int16 int32 int64 uint8 uint16 uint32 uint64 float32 float64
syn keyword clythBoolean true false
syn match clythCustomType "\<[A-Z][A-Za-z0-9_]*\>"
syn match clythImportModule "^\s*import\s\+\zs[A-Za-z_][A-Za-z0-9_-]*\%(/[A-Za-z0-9_.-]\+\)*"
syn match clythImportFile "^\s*import\s\+\zs\"[^\"]\+\""
syn match clythCallableType "\<[A-Za-z_][A-Za-z0-9_]*\>\s*(\s*[A-Za-z_][A-Za-z0-9_]*\%([[:space:]]*,[[:space:]]*[A-Za-z_][A-Za-z0-9_]*\)*\s*)\%([[:space:]]*\[\]\)*\s\+[A-Za-z_][A-Za-z0-9_]*"
syn match clythLambdaArrow "=>"
syn match clythNumber "\<\(0[xX][0-9A-Fa-f]\+\|0[bB][01]\+\|[0-9]\+\(\.[0-9]\+\)\?\)\>"
syn region clythString start=+"+ skip=+\\.+ end=+"+
syn match clythComment "//.*$"
syn region clythComment start="/\*" end="\*/"
hi def link clythControl Keyword
hi def link clythDecl Statement
hi def link clythType Type
hi def link clythCustomType Type
hi def link clythImportModule Include
hi def link clythImportFile String
hi def link clythCallableType Type
hi def link clythBoolean Boolean
hi def link clythLambdaArrow Operator
hi def link clythNumber Number
hi def link clythString String
hi def link clythComment Comment
let b:current_syntax = "clyth"

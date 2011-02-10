
;           Copyright Oliver Kowalke 2009.
;  Distributed under the Boost Software License, Version 1.0.
;     (See accompanying file LICENSE_1_0.txt or copy at
;           http://www.boost.org/LICENSE_1_0.txt)

;  --------------------------------------------------------------
;  |    0    |    1    |    2    |    3    |    4     |    5    |
;  --------------------------------------------------------------
;  |    0h   |   04h   |   08h   |   0ch   |   010h   |   014h  |
;  --------------------------------------------------------------
;  |   EBX   |   EBP   |   EDI   |   ESI   |   ESP    |   EIP   |
;  --------------------------------------------------------------
;  --------------------------------------------------------------
;  |    6    |    7    |                                        |
;  --------------------------------------------------------------
;  |   018h  |   01ch  |                                        |
;  --------------------------------------------------------------
;  |fc_x87_cw| ss_base |                                        |
;  --------------------------------------------------------------
;  --------------------------------------------------------------
;  |    8    |    9    |   10    |   11    |    12    |    13   |
;  --------------------------------------------------------------
;  |  020h   |  024h   |  028h   |  02ch   |                    |
;  --------------------------------------------------------------
;  | ss_limit| fc_link |excpt_lst|fc_strge |                    |
;  --------------------------------------------------------------

.386
.model flat, c
_exit PROTO, value:SDWORD           ; standard C library function
.code
get_fcontext PROC
    mov     eax,         [esp+04h]  ; load address of the fcontext_t
    mov     [eax],       ebx        ; save EBX
    mov     [eax+04h],   ebp        ; save EBP
    mov     [eax+08h],   edi        ; save EDI
    mov     [eax+0ch],   esi        ; save ESI

    assume  fs:nothing
    mov     edx,         fs:[018h]  ; load NT_TIB
    assume  fs:error
    mov     ecx,         [edx]      ; load current SEH exception list
    mov     [eax+028h],  ecx        ; save current exception list
    mov     ecx,         [edx+04h]  ; load current stack base
    mov     [eax+01ch],  ecx        ; save current stack base
    mov     ecx,         [edx+08h]  ; load current stack limit
    mov     [eax+020h],  ecx        ; save current stack limit
    mov     ecx,         [edx+010h] ; load fiber local storage
    mov     [eax+02ch],  ecx        ; save fiber local storage

    fnstcw  [eax+018h]              ; save x87 control word

    lea     ecx,         [esp+04h]  ; exclude the return address
    mov     [eax+010h],  ecx        ; save as stack pointer
    mov     ecx,         [esp]      ; load return address
    mov     [eax+014h],  ecx        ; save return address

    xor     eax,         eax        ; set return value to zero
    ret                             ; return
get_fcontext ENDP

set_fcontext PROC
    mov     eax,        [esp+04h]   ; load address of the fcontext_t arg
    mov     ebx,        [eax]       ; restore EBX
    mov     ebp,        [eax+04h]   ; restore EBP
    mov     edi,        [eax+08h]   ; restore EDI
    mov     esi,        [eax+0ch]   ; restore ESI

    assume  fs:nothing
    mov     edx,        fs:[018h]   ; load NT_TIB
    assume  fs:error
    mov     ecx,        [eax+028h]  ; load SEH exception list
    mov     [edx],      ecx         ; restore next SEH item
    mov     ecx,        [eax+01ch]  ; load stack base
    mov     [edx+04h],  ecx         ; restore stack base
    mov     ecx,        [eax+020h]  ; load stack limit
    mov     [edx+08h],  ecx         ; restore stack limit
    mov     ecx,        [eax+02ch]  ; load fiber local storage
    mov     [edx+010h], ecx         ; restore fiber local storage

    fldcw   [eax+018h]              ; restore x87 control word

    mov     esp,        [eax+010h]  ; restore ESP
    mov     ecx,        [eax+014h]  ; fetch the address to return to
    push    ecx                     ; push the return address on the new stack so we can return there

    xor     eax,        eax         ; set return value to zero
    ret                             ; 'ret' will pop the return address of the code and jump to it
set_fcontext ENDP

swap_fcontext PROC
    mov     eax,         [esp+04h]  ; load address of the first fcontext_t arg
    mov     [eax],       ebx        ; save EBX
    mov     [eax+04h],   ebp        ; save EBP
    mov     [eax+08h],   edi        ; save EDI
    mov     [eax+0ch],   esi        ; save ESI

    assume  fs:nothing
    mov     edx,         fs:[018h]  ; load NT_TIB
    assume  fs:error
    mov     ecx,         [edx]      ; load current SEH exception list
    mov     [eax+028h],  ecx        ; save current exception list
    mov     ecx,         [edx+04h]  ; load current stack base
    mov     [eax+01ch],  ecx        ; save current stack base
    mov     ecx,         [edx+08h]  ; load current stack limit
    mov     [eax+020h],  ecx        ; save current stack limit
    mov     ecx,         [edx+010h] ; load fiber local storage
    mov     [eax+02ch],  ecx        ; save fiber local storage

    fnstcw  [eax+018h]              ; save x87 control word

    lea     ecx,         [esp+04h]  ; exclude the return address
    mov     [eax+010h],  ecx        ; save as stack pointer
    mov     ecx,         [esp]      ; load return address
    mov     [eax+014h],  ecx        ; save return address


    mov     eax,        [esp+08h]   ; load address of the second fcontext_t arg
    mov     ebx,        [eax]       ; restore EBX
    mov     ebp,        [eax+04h]   ; restore EBP
    mov     edi,        [eax+08h]   ; restore EDI
    mov     esi,        [eax+0ch]   ; restore ESI

    assume  fs:nothing
    mov     edx,        fs:[018h]   ; load NT_TIB
    assume  fs:error
    mov     ecx,        [eax+028h]  ; load SEH exception list
    mov     [edx],      ecx         ; restore next SEH item
    mov     ecx,        [eax+01ch]  ; load stack base
    mov     [edx+04h],  ecx         ; restore stack base
    mov     ecx,        [eax+020h]  ; load stack limit
    mov     [edx+08h],  ecx         ; restore stack limit
    mov     ecx,        [eax+02ch]  ; load fiber local storage
    mov     [edx+010h], ecx         ; restore fiber local storage

    fldcw   [eax+018h]              ; restore x87 control word

    mov     esp,        [eax+010h]  ; restore ESP
    mov     ecx,        [eax+014h]  ; fetch the address to return to
    push    ecx                     ; push the return address on the new stack so we can return there

    xor     eax,        eax         ; set return value to zero
    ret                             ; 'ret' will pop the return address of the code and jump to it
swap_fcontext ENDP

make_fcontext PROC
    mov  eax,         [esp+04h]     ; load address of the fcontext_t arg0
    mov  ecx,         [esp+08h]     ; load the address of the function supposed to run
    mov  [eax+014h],  ecx           ; save the address of the function supposed to run
    mov  edx,         [eax+01ch]    ; load the stack base
    lea  edx,         [edx-010h]    ; reserve space for the last frame on stack
    mov  [eax+010h],  edx           ; save the address

    mov  ecx,         0ffffffffh    ; set ECX to -1
    mov  [edx+0ch],   ecx           ; save -1 as SEH handler
    mov  [edx+08h],   ecx           ; save -1 as next SEH item
    lea  ecx,         [edx+08h]     ; load address of next SEH item
    mov  [eax+028h],  ecx           ; save next SEH

    mov  ecx,         [eax+024h]    ; load the address of the next context
    mov  [eax],       ecx           ; save the address of the next context
    mov  ecx,         [esp+0ch]     ; load the address of the void pointer arg2
    mov  [edx+04h],   ecx           ; save the address of the void pointer onto the context stack
    mov  ecx,         link_fcontext ; load helper code executed after fn() returns
    mov  [edx],       ecx           ; save helper code executed adter fn() returns
    xor  eax,         eax           ; set return value to zero
    ret                             ; return 0
make_fcontext ENDP

link_fcontext PROC
    push  ebx
    test  ebx,        ebx           ; test if a next context was given
    je    finish                    ; jump to finish
    call  set_fcontext              ; install next context
finish:
    call  _exit                     ; exit application
link_fcontext ENDP
END

global  main
global strlen
extern printf
extern puts

SECTION .rodata

SYS_READ: dd 3
SYS_WRITE: dd 4
SYS_OPEN: dd 5
SYS_CLOSE: dd 6
SYS_FSEEK: dd 19
STDOUT: dd 1

ENDLINE: db 0

repr: db "repr: %d", 10, 0

SECTION .data

rv: dd 0
buf: db 0

input: dd 0
output: dd 1
key: dd 0
key_size: dd 0
key_index: dd 0

SECTION .text
main:
    push ebp
    mov ebp, esp
    mov ebx, [ebp + 8] ; argc
    mov esi, [ebp + 12] ; argv[0]
    loop:
        pusha
        push dword [esi] ; argv[i]
        call parse_argument
        add esp, 4
        mov [rv], eax
        popa
        mov eax, [rv]


        add esi, 4 ; next argv
        dec ebx
        jne loop 

    pusha
    call write_file
    popa

    pusha
    call close_files
    popa

    mov eax, 0
    leave
    ret

; () -> void
write_file:
    push ebp
    mov ebp, esp

    write_file_loop:
        pusha
        push dword 1
        push dword buf
        push dword [input]
        push dword [SYS_READ]
        call system_call
        add esp, 16
        mov [rv], eax
        popa
        mov eax, [rv]
        cmp eax, 1
        jne write_file_loop_end

        pusha
        call encode_buf
        popa

        pusha
        push dword 1
        push dword buf
        push dword [output]
        push dword [SYS_WRITE]
        call system_call
        add esp, 16
        popa

        jmp write_file_loop
    write_file_loop_end:

    pop ebp
    ret

encode_buf:
    push ebp
    mov ebp, esp

    mov eax, [key_size]
    cmp eax, 0
    je encode_buf_done

    mov eax, [key]
    mov ebx, [key_index]
    add eax, ebx
    mov ebx, 0
    mov bl, [eax] ; bl <- current key char
    sub bl, '0' ; bl <- current key int

    mov al, [buf]
    add al, bl
    mov [buf], al

    mov eax, [key_index] ; key_index <- key_index + 1 % key_size
    inc eax
    cmp eax, [key_size]
    jne encode_buf_key_no_reset
    mov eax, 0
    encode_buf_key_no_reset:
    mov [key_index], eax

    encode_buf_done:
    pop ebp
    ret

; (char[] arg) -> void
parse_argument:
    push ebp
    mov ebp, esp

    mov ebx, [ebp + 8]
    mov bl, [ebx] ; ebx <- first char of arg
    cmp bl, '-'
    jnz parse_argument_not_minus
        ; Argument starts with '-'
        mov ebx, [ebp + 8]
        mov bl, [ebx + 1] ; ebx <- second char of arg
        cmp bl, 'i'
        jnz parse_argument_not_input
        ; Argument is '-i'
        pusha
        push dword [ebp + 8]
        call parse_input_argument
        add esp, 4
        popa

        parse_argument_not_input:
        cmp bl, 'o'
        jnz parse_argument_neither
        ; Argument is '-o'
        pusha
        push dword [ebp + 8]
        call parse_output_argument
        add esp, 4
        popa

    parse_argument_not_minus:
    cmp bl, '+'
    jnz parse_argument_neither
        ; Argument starts with '+'
        mov ebx, [ebp + 8]
        mov bl, [ebx + 1] ; ebx <- second char of arg
        cmp bl, 'e'
        jnz parse_argument_neither
        ; Argument is '+e'
        pusha
        push dword [ebp + 8]
        call parse_encoding_argument
        add esp, 4
        popa

    parse_argument_neither:
    ; Handle errors? It's asm...
    pop ebp
    ret

; (string argument) -> void
parse_input_argument:
    push ebp
    mov ebp, esp

    mov eax, [ebp + 8]
    add eax, 2 ; truncate first two letters '-i'

    pusha
    push dword 0666
    push dword 0 ; O_RDONLY mode
    push dword eax
    push dword [SYS_OPEN]
    call system_call
    add esp, 16
    mov [input], eax
    popa

    pop ebp
    ret

; (string argument) -> void
parse_output_argument:
    push ebp
    mov ebp, esp

    mov eax, [ebp + 8]
    add eax, 2 ; truncate first two letters '-i'

    pusha
    push dword 440
    push dword 65 ; O_WRONLY and O_CREAT access bits.
    push dword eax
    push dword [SYS_OPEN]
    call system_call
    add esp, 16
    mov [output], eax
    popa

    pop ebp
    ret

; (string argument) -> void
parse_encoding_argument:
    push ebp
    mov ebp, esp

    mov eax, [ebp + 8]
    add eax, 2 ; truncate first two letters '-i'
    mov [key], eax

    pusha
    push dword [key]
    call strlen
    add esp, 4
    mov [key_size], eax
    popa

    pop ebp
    ret

; () -> void
close_files:
    push ebp
    mov ebp, esp

    pusha
    push dword 0
    push dword 0
    push dword [input]
    push dword [SYS_CLOSE]
    call system_call
    add esp, 16
    popa

    pusha
    push dword 0
    push dword 0
    push dword [output]
    push dword [SYS_CLOSE]
    call system_call
    add esp, 16
    popa

    pop ebp
    ret

; (char[] null_terminated) -> int
strlen:
    push ebp
    mov ebp, esp

    mov eax, [ebp + 8] ; eax <- null_terminated
    strlen_counter:
        mov cl, [eax]
        inc eax
        cmp cl, 0
        jne strlen_counter

    sub eax, [ebp + 8] ; eax <- length
    dec eax ; because the loop increments before finishing.
    pop ebp
    ret

system_call:
    push    ebp             ; Save caller state
    mov     ebp, esp
    sub     esp, 4          ; Leave space for local var on stack
    pushad                  ; Save some more caller state

    mov     eax, [ebp+8]    ; Copy function args to registers: leftmost...        
    mov     ebx, [ebp+12]   ; Next argument...
    mov     ecx, [ebp+16]   ; Next argument...
    mov     edx, [ebp+20]   ; Next argument...
    int     0x80            ; Transfer control to operating system
    mov     [ebp-4], eax    ; Save returned value...
    popad                   ; Restore caller state (registers)
    mov     eax, [ebp-4]    ; place returned value where caller can see it
    add     esp, 4          ; Restore caller state
    pop     ebp             ; Restore caller state
    ret                     ; Back to caller

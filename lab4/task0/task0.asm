    global main
    extern printf
    extern  puts
    section .data
    str: db "argc: %d.", 10, "argv[0]: ", 0
    section .text
main:
    push ebp
    mov ebp, esp
    mov edx, [ebp + 8] ;argc
    push edx
    push dword str
    call printf
    mov edx, [ebp + 12] ; argv[0]
    push dword[edx]
    call puts
    leave
    ret
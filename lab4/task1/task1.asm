SECTION  .data
    argc: db "argc: %d.", 10, "argv:", 10, 0
SECTION .text
global  main
extern printf
extern puts
main:
    push ebp
    mov ebp, esp
    mov ebx, [ebp + 8] ; argc ;argc
    push ebx
    push argc
    call printf
    mov esi, [ebp + 12] ; argv[0]
    loop:
        cmp ebx, 0 ;are we finish all the argc?
        je end
        mov eax, [esi] ;next argv
        push eax 
        call puts
        add esi, 4 ; next argv
        dec ebx ;decrease argc
        jmp loop 
    end:
        leave
        ret
    
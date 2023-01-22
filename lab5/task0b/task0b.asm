global main
global strlen
global system_call
extern printf
extern  puts
section .data
    enter: db 10, 0
section .text
main: 
    push ebp
    mov ebp, esp
    mov ebx, [ebp + 8] ; argc
    mov esi, [ebp + 12] ; argv[0]
    loop:
        cmp ebx, 0 ; are we finish?
        je end
        pusha
        push dword [esi] ;check word length
        call strlen
        add esp, 4
        mov edx, eax ; length
        mov eax, esi ; argv[i]
        push dword edx ;length
        push dword [eax] ;argv[i]
        push dword 1 ; stdout
        push dword 4 ;write
        call system_call
        add esp, 16
        push dword 1 ;print enter
        push dword enter
        push dword 1
        push dword 4
        call system_call
        add esp, 16
        popa
        add esi, 4 ; next argv
        dec ebx
        jmp loop 
    end:
        mov eax, 1
        mov ebx, 0
        int 0x80

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
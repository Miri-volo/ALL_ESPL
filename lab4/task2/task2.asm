global main
global print_multi
extern exit
extern printf
extern puts

section .rodata
repr: db "%02hhx", 0
enter: db 10,0

x_struct: dd 5
x_num: db 0xaa, 1, 2, 0x44, 0x4f

section .text
main:
	mov ebp, esp
	push x_struct
	call print_multi
	push 0
	call exit

print_multi:
	push ebp
	mov ebp, esp
	mov eax, [ebp + 8] ;pointer to the struct
	mov ebx, [eax] ; ebx = len of number

	add eax, 4 ; eax = num[0]
	add eax, ebx ; eax = num[last+1]
	dec eax ; The array starts from 0.

	LOOP:
		pusha
		push dword [eax] ; num[i]
		push dword repr  ; format string
		call printf
		add esp, 8
		popa
		dec eax ; num[i-1]
		dec ebx ; count--
		jne LOOP
	push enter ; prettify the print
	call printf
	add esp,4
	pop ebp
	ret

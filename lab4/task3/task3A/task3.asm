global main
global print_multi
global maxmin
extern exit
extern printf
extern puts

section .rodata
enter: db 10,0
repr: db "%02hhx", 0
x_struct: dd 5
x_num: db 0xaa, 1,2,0x44,0x4f
y_struct: dd 6
y_num: db 0xaa, 1,2,3,0x44,0x4f

section .text
main:
	mov ebp, esp
	push x_struct
	push y_struct
	call maxmin
	add esp,8
	push eax 
	call print_multi
	add esp,4
	push ebx 
	call print_multi
	add esp,4
	push 0
	call exit

print_multi:
	push ebp
	mov ebp, esp
	mov ecx, [ebp + 8] ;pointer to the struct
	mov edx, [ecx] ; ebx = len of number

	add ecx, 4 ; eax = num[0]
	add ecx, edx ; eax = num[last+1]
	dec ecx ; The array starts from 0.

	LOOP:
		pusha
		push dword [ecx] ; num[i]
		push dword repr ; format string
		call printf
		add esp, 8
		popa
		dec ecx ; num[i-1]
		dec edx ; count--
		jne LOOP
	push enter ; prettify the print
	call printf
	add esp, 4
	pop ebp
	ret

maxmin:
	push ebp
	mov ebp, esp
	mov eax, [ebp + 8] ;pointer to the first struct
	mov eax, [eax] ; eax = len of first struct
	mov ebx, [ebp + 12] ;pointer to the second struct
	mov ebx, [ebx] ; ebx = len of the second struct
	cmp eax, ebx ;if first struct smaller?
	jl little
	mov eax, [ebp + 8] ;first struct is bigger
	mov ebx, [ebp + 12] ;second struct is smaller
	jmp end
	little:
		mov ebx, [ebp + 8] ;first struct is smaller
		mov eax, [ebp + 12] ;second struct is bigger
	end:
		pop ebp
		ret
        
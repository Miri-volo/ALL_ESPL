global main
global print_multi
global maxmin
global add_multi
extern exit
extern printf
extern malloc
extern free
extern puts

section .rodata
enter: db 10,0
argc: db "%d", 10,0
repr: db "%02hhx", 0
adding: db "%02hhx + %02hhx = %02hhx", 10, 0
x_struct: dd 6
    x_val: db 0xff,0xff,0xff,0xff,0xff,0
y_struct: dd 5
    y_val: db 1,0,0,0,0

section .data
differ: dd 0
array: dd 0

section .text
main:
	mov ebp, esp
	push x_struct
	push y_struct
	call add_multi
	add esp, 8
	pusha
	push eax
	call print_multi
	add esp, 4
	popa
	push eax
	call free
	add esp, 4
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
        
add_multi:
	push ebp
	mov ebp, esp
	mov ecx, [ebp + 8] ;pointer to the first struct
    mov edx, [ebp + 12] ;pointer to the second struct
	push ecx
	push edx
	call maxmin ;call maxmin
	add esp, 8
	mov edx, [eax] ;biggest array length
	add edx, 4
	pusha
	push edx
	call malloc ;add malloc to eax
	add esp, 4
	mov [array], eax ;move pointer of malloc to [array]
	popa
	mov ecx, [eax]
	mov esi, [array]
	mov [esi], ecx ;adding struct size to the new array
	mov ecx, [ebx]
	mov esi, [eax]
	mov [differ], esi
	mov esi, [ebx]
	sub [differ], esi ; adding struct length difference to differ.
	add ebx, 4 ;first element in smaller struct
	add eax, 4 ;first element in larger struct
	mov edi, [array] ; pointer to struct output
	add edi, 4 ; first element in outpus
	mov esi, 0 ; carry
	same_length:
		add esi, [ebx] ; add element from smaller struct to existing carry.
		and esi, 0x000000ff ; mask to keep single byte from number.
		mov [edi], esi ; add element to output struct.
		mov esi, [eax] ; larger struct.
		and esi, 0x000000ff
		add [edi], esi ; add element from larger struct to output struct.
		mov esi, [edi]
		shr esi, 8 ; shift result to set esi to current carry.
		pusha
		push dword [edi] ; pretty print
		push dword [ebx]
		push dword [eax]
		push adding
		call printf
		add esp, 16
		popa
		inc eax ; increment larger struct pointer.
		inc ebx ; increment smaller struct pointer.
		inc edi ; increment pointer to output struct.
		dec ecx ; decrease loop counter.
		jne same_length
	mov ecx, [differ] ; set loop counter to size difference.
	cmp ecx, 0 ; finish if struct sizes are the same.
	je finish
	big_struct_loop:
		add esi, [eax] ; add larger struct element and carry.
		and esi, 0x000000ff ; mask to keep single byte from number.
		mov [edi], esi ; move result into output struct.
		mov esi, [edi]
		shr esi, 8 ; handle carry.
		pusha
		push dword [edi] ; pretty print.
		push 0
		push dword [eax]
		push adding
		call printf
		add esp, 16
		popa
		inc eax ; increment larger struct pointer.
		inc edi ; increment pointer to output struct.
		dec ecx ; decrease loop counter.
		jne big_struct_loop
	finish:
		mov eax, [array] ; move allocated array pointer to return value.
		pop ebp
		ret

global main
global rand_num
extern exit
extern printf

section .rodata
repr: db "%x", 10,0
MASK: dq 0b1000011111101101

section .data
STATE: dq 0x1337456


section .text
main:
	mov ecx, 20
	LOOP:
		pusha
		call rand_num
		push eax
		push repr
		call printf
		add esp, 8
		popa
		dec ecx
		jne LOOP
	push 0
	call exit

rand_num:
	push ebp
	; eax <- STATE & MASK
	mov ebp, esp
	mov eax, [STATE]
	and eax, [MASK]
	
	; ebx <- parity flag.
	pushf
	pop ebx
	shr ebx, 2
	and ebx, 1

	; Shift state and set MSB to parity flag.
	mov eax, [STATE]
	shr eax, 1
	shl ebx, 15 ; Shift parity bit to MSB.
	or eax, ebx ; Set MSB of STATE to parity bit.
	mov [STATE], eax ; Set state.
	pop ebp
	ret

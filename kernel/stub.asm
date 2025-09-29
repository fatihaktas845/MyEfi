BITS 64

section .header alloc
	align 8
	dq _start

section .text
global _start

extern kmain

_start:
	mov rdi, rcx
	mov rsi, rdx
	call kmain

.halt:
	hlt
	jmp .halt

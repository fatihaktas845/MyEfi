BITS 64

section .text
global _start
extern kmain

_start:
	mov rdi, rcx
	call kmain

.halt:
	hlt
	jmp .halt

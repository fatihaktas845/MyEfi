section .text

extern kmain

global _start

_start:
    cld
	cli
	mov		rdi, rcx
    mov     rsp, 0x00A00000

	mov		rax, kmain
	jmp		rax

.hang:
    cli
    hlt
    jmp .hang

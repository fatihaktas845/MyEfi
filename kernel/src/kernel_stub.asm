section .text

extern kmain
extern __stack_end

global _start

_start:
    cld
	cli
	mov		rdi, rcx
    mov     rsp, __stack_end

	mov		rax, kmain
	jmp		rax

.hang:
    cli
    hlt
    jmp .hang

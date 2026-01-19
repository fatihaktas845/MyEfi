section .text.boot

extern kmain
extern setup_paging

global _start

_start:
    cld
	cli
	mov		rdi, rcx
    mov     rsp, 0x00A00000
    call    setup_paging

	mov		rax, kmain
	jmp		rax

.hang:
    cli
    hlt
    jmp .hang

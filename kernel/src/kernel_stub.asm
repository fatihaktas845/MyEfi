section .text.boot

extern kmain
extern setup_paging

global _start

_start:
    cld

	mov		al, 'C'
	out		0x3F8, al
    mov     r12, rcx
    mov     rsp, 0x00A00000
    call    setup_paging

	mov		rdi, r12

	mov		rax, kmain
	call	rax

.hang:
    cli
    hlt
    jmp .hang

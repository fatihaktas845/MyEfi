section .text.boot

extern kmain
extern setup_paging

global _start

_start:
    cld

	mov		rdx, 0x00000666

    mov     rsp, 0x00A00000
	mov		rdx, 0x00000555
    call    setup_paging
	mov		rdx, 0x00000444

	mov		rax, kmain
	call	rax

.hang:
    cli
    hlt
    jmp .hang

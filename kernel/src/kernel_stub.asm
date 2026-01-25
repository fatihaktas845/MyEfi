section .text

extern initGDT
extern gdtPtr

extern __stack_end

extern kmain
extern bootInfo

global _start

_start:
    cld
	cli
	mov		[bootInfo], rcx
    mov     rsp, __stack_end

	call	initGDT
	lgdt	[gdtPtr]

	; far jump for CS Reload
	push	0x08
	lea		rax, [.after_cs_reload]
	push	rax
	retfq

.after_cs_reload:
	mov		ax, 0x10
	mov		ds, ax
	mov		es, ax
	mov		ss, ax

	xor		ax, ax
	mov		fs, ax
	mov		gs, ax

	jmp		kmain

.hang:
    cli
    hlt
    jmp .hang

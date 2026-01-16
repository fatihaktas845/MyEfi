extern kmain
global _start

section .text

_start:
    cld
    mov     rdi, rcx
    mov     rsp, 0x00A00000
    and     rsp, -16
    push    0
    call    kmain

.hang:
    cli
    hlt
    jmp .hang

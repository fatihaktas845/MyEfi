/* kernel.ld */
OUTPUT_FORMAT(elf64-x86-64)
ENTRY(_start)

SECTIONS
{
    . = 2M;

    .text ALIGN(2M) : {
        *(.text*)
    }

    .rodata ALIGN(2M) : {
        *(.rodata*)
    }

    .data ALIGN(2M) : {
        *(.data*)
    }

    .bss ALIGN(2M) : {
        *(COMMON)
        *(.bss*)
    }
}

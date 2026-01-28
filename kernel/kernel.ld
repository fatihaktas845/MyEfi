OUTPUT_FORMAT(elf64-x86-64)
ENTRY(_start)

KERNEL_PHYS_BASE = 0x00200000;
KERNEL_VIRT_BASE = 0xFFFFFFFF80000000;

SECTIONS
{
    . = KERNEL_VIRT_BASE;

    .text ALIGN(0x1000) :
    {
        *(.text*)
    }

    .rodata ALIGN(0x1000) :
    {
        *(.rodata*)
    }

    .data ALIGN(0x1000) :
    {
        *(.data*)
    }

    .bss ALIGN(0x1000) :
    {
        *(.bss*)
        *(COMMON)
    }

	.stack ALIGN(0x1000) :
	{
		__stack_start = .;
		*(.stack*)
		. += 16K;
		__stack_end = .;
	}

	.heap ALIGN(0x1000) :
	{
		__kernel_heap_start = .;
		*(.heap*)
		. += 4M;
	}
}

OUTPUT_FORMAT(elf64-x86-64)
ENTRY(_start)

KERNEL_PHYS_BASE = 0x00200000;
KERNEL_VIRT_BASE = 0xFFFFFFFF80000000;

SECTIONS
{
    . = KERNEL_VIRT_BASE;

    .text ALIGN(0x1000) :
    {
        KEEP(*(.text*))
    }

    .rodata ALIGN(0x1000) :
    {
        KEEP(*(.rodata*))
    }

    .data ALIGN(0x1000) :
    {
        KEEP(*(.data*))
    }

    .bss ALIGN(0x1000) :
    {
        KEEP(*(.bss*))
        KEEP(*(COMMON))
    }

	.stack ALIGN(0x1000) :
	{
		__stack_start = .;
		KEEP(*(.stack*))
		. += 16K;
		__stack_end = .;
	}

	.heap ALIGN(0x1000) :
	{
		__kernel_heap_start = .;
		KEEP(*(.heap*))
		. += 4M;
	}
}

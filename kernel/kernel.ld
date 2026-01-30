OUTPUT_FORMAT(elf64-x86-64)
ENTRY(_start)

KERNEL_PHYS_BASE = 0x00200000;
KERNEL_VIRT_BASE = 0xFFFFFFFF80000000;

PHDRS
{
	text PT_LOAD FLAGS(5);
	data PT_LOAD FLAGS(6);
}

SECTIONS
{
    . = KERNEL_VIRT_BASE;

    .text ALIGN(0x1000) :
    {
        KEEP(*(.text*))
    } :text

    .rodata ALIGN(0x1000) :
    {
        KEEP(*(.rodata*))
    } :data

    .data ALIGN(0x1000) :
    {
        KEEP(*(.data*))
    } :data

    .bss ALIGN(0x1000) (NOLOAD):
    {
        KEEP(*(.bss*))
        KEEP(*(COMMON))
    }

	.stack ALIGN(0x1000) (NOLOAD):
	{
		__stack_start = .;
		KEEP(*(.stack*))
		. += 16K;
		__stack_end = .;
	}

	.heap ALIGN(0x1000) (NOLOAD):
	{
		__kernel_heap_start = .;
		KEEP(*(.heap*))
		. += 4M;
	}
}

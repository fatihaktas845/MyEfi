OUTPUT_FORMAT(elf64-x86-64)
ENTRY(_start)

KERNEL_PHYS_BASE = 0x00200000;
KERNEL_VIRT_BASE = 0xFFFFFFFF80000000;

/* PHDRS
{
    boot_text PT_LOAD FLAGS(5);
    boot_data PT_LOAD FLAGS(6);
    kern_text PT_LOAD FLAGS(5);
    kern_data PT_LOAD FLAGS(6);
} */

SECTIONS
{
    /* =====================================================
     *  BOOT / PAGING ÖNCESİ (LOW VA, IDENTITY MAPPED)
     * ===================================================== */
    . = KERNEL_PHYS_BASE;

    .text.boot ALIGN(0x1000) :
    {
        KEEP(*(.text.boot))
    }

    .rodata.boot ALIGN(0x1000) :
    {
        *(.rodata.boot)
    }

    .data.boot ALIGN(0x1000) :
    {
        *(.data.boot)
    }

    .bss.boot ALIGN(0x1000) : AT(0x00300000)
    {
        *(.bss.boot)
        *(COMMON)
    }

	_boot_end = 0x00400000;


    /* =====================================================
     *  HIGH-HALF KERNEL
     * ===================================================== */
    . = KERNEL_VIRT_BASE;

    .text ALIGN(0x1000) : AT(_boot_end)
    {
        *(.text*)
    }

    .rodata ALIGN(0x1000) : AT(_boot_end + SIZEOF(.text))
    {
        *(.rodata*)
    }

    .data ALIGN(0x1000) : AT(_boot_end + SIZEOF(.text) + SIZEOF(.rodata))
    {
        *(.data*)
    }

    .bss ALIGN(0x1000) : AT(_boot_end + SIZEOF(.text) + SIZEOF(.rodata) + SIZEOF(.data))
    {
        *(.bss*)
    }
}

OUTPUT_FORMAT(elf64-x86-64)
ENTRY(_start)

/* =====================================================
 *  SABİT ADRESLER
 * ===================================================== */
KERNEL_PHYS_BASE = 0x00200000;
KERNEL_VIRT_BASE = 0xFFFFFFFF80000000;
KERNEL_OFFSET    = KERNEL_VIRT_BASE - KERNEL_PHYS_BASE;

SECTIONS
{
    /* =====================================================
     *  BOOT / EARLY STAGE (LOW, IDENTITY MAPPED)
     * ===================================================== */
    . = KERNEL_PHYS_BASE;

    __boot_start = .;

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

    .bss.boot ALIGN(0x1000) :
    {
        *(.bss.boot)
        *(COMMON)
    }

    __boot_end = .;

    /* =====================================================
     *  HIGH-HALF KERNEL
     * ===================================================== */
    . = KERNEL_VIRT_BASE + (__boot_end - KERNEL_PHYS_BASE);

    __kernel_start = .;

    .text ALIGN(0x1000) : AT(ADDR(.text) - KERNEL_OFFSET)
    {
        *(.text*)
    }

    .rodata ALIGN(0x1000) : AT(ADDR(.rodata) - KERNEL_OFFSET)
    {
        *(.rodata*)
    }

    .data ALIGN(0x1000) : AT(ADDR(.data) - KERNEL_OFFSET)
    {
        *(.data*)
    }

    .bss ALIGN(0x1000) : AT(ADDR(.bss) - KERNEL_OFFSET)
    {
        *(.bss*)
        *(COMMON)
    }

    __kernel_end = .;
}

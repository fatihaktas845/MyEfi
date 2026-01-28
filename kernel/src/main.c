#include "bootInfo.h"
#include "psf.h"

static inline void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile ( "outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

uint8_t* hex64_to_str(uint64_t hex) {
    static uint8_t buf[17]; // 16 hex digit + '\0'

    for (int i = 15; i >= 0; i--) {
        uint8_t nibble = hex & 0xF;
        buf[i] = (nibble < 10) ? ('0' + nibble) : ('A' + nibble - 10);
        hex >>= 4;
    }

    buf[16] = '\0';
    return buf;
}

void *bootInfo;

void kmain() {
	BootInfo *bi = (BootInfo *)bootInfo;
	// outb(0x3F8, 'K');
	volatile uint32_t *fbb = (uint32_t*)bi->kgi->FrameBufferBase;
	uint32_t pps = bi->kgi->PixelsPerScanLine;

	initPSFv1(fbb, pps);
	printk("Welcome To Veyra!\n", 0x00FFFFFF);
	printk("Hello", 0x00FFFFFF);

	uint64_t rip;
	asm volatile("leaq (%%rip), %0" : "=r"(rip));

	uint8_t *str = hex64_to_str(rip);

	while (*str) {
		outb(0x3F8, *str);
		str++;
	}

	/* int a = 1, b = 0;
	int c = a / b; // To test IDT #DE */

	while (1);
}

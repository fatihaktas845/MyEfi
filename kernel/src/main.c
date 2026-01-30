#include "bootInfo.h"
#include "psf.h"
#include "memory.h"
#include "utils.h"

static inline void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile ( "outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

char* hex64_to_str(uint64_t hex) {
    static char buf[17]; // 16 hex digit + '\0'

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
	
	// initPageAllocator();
	// KernelGOPInfo *kgi = (KernelGOPInfo *)allocPage();
	// memcpy(kgi, bi->kgi, 4096);

	volatile uint32_t *fbb = (uint32_t*)bi->kgi->FrameBufferBase;
	uint32_t pps = bi->kgi->PixelsPerScanLine;

	initPSFv1(fbb, pps);
	printk("Welcome To Veyra!\n", 0x00FFFFFF);

	uint64_t rip;
	asm volatile("leaq (%%rip), %0" : "=r"(rip));

	char *str = hex64_to_str(rip);

	printk("RIP: ", 0x0000FF00);
	printk(str, 0x0000FF00);
	printk("\n", 0x00FF0000);

	printk(hex64_to_str(bi->kgi->FrameBufferBase), 0x00FFFF00);
	/* while (*str) {
		outb(0x3F8, *str);
		str++;
	} */

	/* int a = 1, b = 0;
	int c = a / b; // To test IDT #DE */

	while (1);
}

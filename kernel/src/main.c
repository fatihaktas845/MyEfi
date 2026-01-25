#include "bootInfo.h"
#include "psf.h"

static inline void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile ( "outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

void *bootInfo;

void kmain() {
	BootInfo *bi = (BootInfo *)bootInfo;
	// outb(0x3F8, 'K');
	volatile uint32_t *fbb = (uint32_t*)bi->kgi->FrameBufferBase;
	uint32_t pps = bi->kgi->PixelsPerScanLine;

	initPSFv1(fbb, pps);
	printString("Welcome To Veyra!", 200, 300, 7, 0x00FFFFFF);

	/* int a = 1, b = 0;
	int c = a / b; // To test IDT #DE */

	while (1);
}

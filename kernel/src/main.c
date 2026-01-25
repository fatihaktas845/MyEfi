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

	initPSFv1();
	drawChar('A', 200, 300, fbb, pps, 16);

	/* for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 100; j++) {
			fbb[i + j *pps] = 0x00FF0000;
		}
	} */

	while (1);
}

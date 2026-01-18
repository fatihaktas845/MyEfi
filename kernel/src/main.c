#include "bootInfo.h"

uint32_t pps;
volatile uint32_t *fbb;

void kmain(BootInfo *bi) {
	fbb = (uint32_t*)(uintptr_t)bi->kgi->FrameBufferBase;
	pps = bi->kgi->PixelsPerScanLine;
	
	for (uint32_t y = 0; y < 100; y++) {
		for (uint32_t x = 0; x < 100; x++) {
			fbb[y * pps + x] = 0x00FF0000;
		}
	}

	while (1);
}

#include "bootInfo.h"

uint32_t pps;
volatile uint32_t *fbb;

void kmain(BootInfo *bi) {
	fbb = (uint32_t*)(uintptr_t)bi->kgi->FrameBufferBase;
	pps = bi->kgi->PixelsPerScanLine;
	
	for (uint32_t y = 0; y < bi->kgi->Height; y++) {
		for (uint32_t x = 0; x < bi->kgi->Width; x++) {
			fbb[y * pps + x] = 0x00FF0000;
		}
	}


	while (1);
}

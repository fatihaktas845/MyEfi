#include <stdint.h>

typedef struct {
	uint32_t Width;
	uint32_t Height;
	uint32_t PixelsPerScanLine;
	uint64_t FrameBufferBase;
} KernelGOPInfo;

uint32_t pps;
volatile uint32_t *fbb;

void kmain(KernelGOPInfo *kgi) {
	fbb = (uint32_t*)(uintptr_t)kgi->FrameBufferBase;
	pps = kgi->PixelsPerScanLine;
	
	for (uint32_t y = 0; y < kgi->Height; y++) {
		for (uint32_t x = 0; x < kgi->Width; x++) {
			fbb[y * pps + x] = 0x00FF0000;
		}
	}

	while (1);
}

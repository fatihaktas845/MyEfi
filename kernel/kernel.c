#include <stdint.h>

uint32_t pixelsPerScanLine;

void __attribute__((ms_abi))  kmain(uint32_t *pps, uint64_t *fb) {
	volatile uint32_t *fbb = (uint32_t*)(uintptr_t)(*fb);
	pixelsPerScanLine = *pps;
	
	for (uint32_t y = 0; y < 50; y++) {
		for (uint32_t x = 0; x < 50; x++) {
			fbb[y * pixelsPerScanLine + x] = 0x00FF0000;
		}
	}

	while (1);
}

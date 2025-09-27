#include <stdint.h>

struct kernel_header {
	uint64_t entry_offset;
};

void kmain(uint32_t *pps, uint64_t *framebufferBase) __attribute__((ms_abi));

__attribute__((section(".header")))
struct kernel_header kheader = {
	.entry_offset = (uint64_t)&kmain
};

void kmain(uint32_t *pps, uint64_t *framebufferBase) {
	uint32_t *fb = (uint32_t*)(uintptr_t)(*framebufferBase);
	uint32_t color = 0x00FF0000;

	for (int y = 0; y < 50; y++) {
		for (int x = 0; x < 50; x++) {
			fb[y * *pps + x] = color;
		}
	}

	while (1);
}

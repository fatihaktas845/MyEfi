#include <stdint.h>

struct kernel_header {
	uint64_t entry_offset;
};

void kmain(void);

__attribute__((section(".header")))
struct kernel_header kheader = {
	.entry_offset = (uint64_t)&kmain
};

void kmain(void) {
	while (1) {}
}

#include <stdint.h>

typedef struct {
	uint32_t Width;
	uint32_t Height;
	uint32_t PixelsPerScanLine;
	uint64_t FrameBufferBase;
} KernelGOPInfo;

uint32_t pps;
volatile uint32_t *fbb;

#define PAGE_PRESENT (1ULL << 0)
#define PAGE_RW      (1ULL << 1)
#define PAGE_PS      (1ULL << 7)

uint64_t pml4[512] __attribute__((aligned(4096)));
uint64_t pdpt[512] __attribute__((aligned(4096)));
// uint64_t pd[512] __attribute__((aligned(4096)));
// uint64_t pt[512][512] __attribute__((aligned(4096)));

void setup_identity_map(KernelGOPInfo *kgi) {
    /* for (int pd_i = 0; pd_i < 512; pd_i++) {
        for (int pt_i = 0; pt_i < 512; pt_i++) {
            uint64_t addr = ((pd_i * 512) + pt_i) * 0x1000;
            pt[pd_i][pt_i] = addr | PAGE_PRESENT | PAGE_RW;
            pd[pd_i] = (uint64_t)&pt[pd_i][0] | PAGE_PRESENT | PAGE_RW;
        }
    } */

	for (int i = 0; i < 512; i++) {
		pdpt[i] = ((uint64_t)i << 30) | PAGE_PRESENT | PAGE_RW | PAGE_PS;
	}

	uint64_t fb_phys = kgi->FrameBufferBase;
	uint64_t fb_aligned = fb_phys & ~((1ULL << 30) - 1);

	uint64_t fb_idx = fb_aligned >> 30; // bits 38..30
	pdpt[fb_idx] = fb_aligned | PAGE_PRESENT | PAGE_RW | PAGE_PS | PAGE_PCD;

    pml4[0] = ((uint64_t)pdpt) | PAGE_PRESENT | PAGE_RW;

    uint64_t pml4_base = (uint64_t)pml4;
    __asm__ volatile(
        "mov %0, %%cr3": : "r"(pml4_base) :
    );

    uint64_t cr0;
    __asm__ volatile(
        "mov %%cr0, %0": "=r"(cr0) : :
    );

    cr0 |= (1ULL << 31);
    __asm__ volatile(
        "mov %0, %%cr0": : "r"(cr0) :
    );
}

void _start() {
	__asm__ volatile (
		"mov %%rcx, %%rdi\n\t"
		"call kmain": : : "rdi"
	);
}

void kmain(KernelGOPInfo *kgi) {
	setup_identity_map(kgi);

	fbb = (uint32_t*)(uintptr_t)kgi->FrameBufferBase;
	pps = kgi->PixelsPerScanLine;
	
	for (uint32_t y = 0; y < kgi->Height; y++) {
		for (uint32_t x = 0; x < kgi->Width; x++) {
			fbb[y * pps + x] = 0x00FF0000;
		}
	}

	while (1);
}

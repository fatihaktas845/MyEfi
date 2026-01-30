#include "paging.h"
#include "utils.h"
#include "memory.h"

static uint64_t *PML4;

void setPML4(uint64_t *_PML4) {
	PML4 = _PML4;
}

void initPML4() {
	PML4 = (uint64_t *)allocPage();
	memset(PML4, 0, 4096);
}

void mapAddress(uint64_t PA, uint64_t VA, uint64_t flags) {
	uint64_t pml4_idx = (VA >> 39) & 0x1FF;
	uint64_t pdpt_idx = (VA >> 30) & 0x1FF;
	uint64_t pd_idx   = (VA >> 21) & 0x1FF;

	uint64_t *PDPT;
	if (PML4[pml4_idx] & PAGE_P) {
		PDPT = (uint64_t *)(PML4[pml4_idx] & ~0xFFF);
	} else {
		PDPT = (uint64_t *)allocPage();
		memset(PDPT, 0, 4096);
		PML4[pml4_idx] = (uint64_t)PDPT | flags;
	}

	uint64_t *PD;
	if (PDPT[pdpt_idx] & PAGE_P) {
		PD = (uint64_t *)(PDPT[pdpt_idx] & ~0xFFF);
	} else {
		PD = (uint64_t *)allocPage();
		memset(PD, 0, 4096);
		PDPT[pdpt_idx] = (uint64_t)PD | flags;
	}

	PD[pd_idx] = PA | flags | PAGE_PS;
}

void loadCR3() {
	uint64_t PML4Base = (uint64_t)PML4;

	asm volatile(
		"mov %0, %%cr3"
		:
		: "r"(PML4Base)
		: "memory"
	);
}

void setupKernelMap() {
	for (uint64_t i = 0; i < 20ULL * 0x100000ULL; i += 0x200000ULL) {
		mapAddress(
				KERNEL_PA + i,
				KERNEL_VA + i,
				PAGE_P | PAGE_RW);
	}
}

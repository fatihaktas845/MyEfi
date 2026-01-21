#include "paging.h"

#define PAGE_PRESENT (1ULL << 0)
#define PAGE_RW      (1ULL << 1)
#define PAGE_PS      (1ULL << 7)

uint64_t *pml4;

void *memset(void *dest, uint8_t value, uint64_t n);

void *allocate_page(EFI_BOOT_SERVICES *bs) {
	EFI_PHYSICAL_ADDRESS addr = 0;
	bs->AllocatePages(
			AllocateAnyPages,
			EfiLoaderData,
			1,
			&addr);

	return (void *)(uint64_t *)addr;
}

void init_pml4(EFI_BOOT_SERVICES *bs) {
	pml4 = (uint64_t *)allocate_page(bs);
	memset(pml4, 0, 4096);
}

void map_address(EFI_BOOT_SERVICES *bs, UINT64 pa, UINT64 va) {
	uint64_t pml4_index = (va >> 39) & 0x1FF;
	uint64_t pdpt_index = (va >> 30) & 0x1FF;
	uint64_t pd_index   = (va >> 21) & 0x1FF;
	// uint64_t pt_index   = (va >> 12) & 0x1FF;

	uint64_t *pdpt;
	if (pml4[pml4_index] & PAGE_PRESENT)
		pdpt = (uint64_t *)(pml4[pml4_index] & ~0xFFF);
	else {
		pdpt = (uint64_t *)allocate_page(bs);
		memset(pdpt, 0, 4096);
		pml4[pml4_index] = (uint64_t)pdpt | PAGE_PRESENT | PAGE_RW;
	}

	uint64_t *pd;
	if (pdpt[pdpt_index] & PAGE_PRESENT)
		pd = (uint64_t *)(pdpt[pdpt_index] & ~0xFFF);
	else {
		pd = (uint64_t *)allocate_page(bs);
		memset(pd, 0, 4096);
		pdpt[pdpt_index] = (uint64_t)pd | PAGE_PRESENT | PAGE_RW;
	}

	pd[pd_index] = pa | PAGE_PRESENT | PAGE_RW | PAGE_PS;

	/* uint64_t *pt;
	if (pd[pd_index] & PAGE_PRESENT)
		pt = (uint64_t *)(pd[pd_index] & ~0xFFF);
	else {
		pt = (uint64_t *)allocate_page(bs);
		memset(pt, 0, 4096);
		pd[pd_index] = (uint64_t)pt | PAGE_PRESENT | PAGE_RW;
	}

	pt[pt_index] = pa | PAGE_PRESENT | PAGE_RW; */
}

void setup_identity_map(EFI_BOOT_SERVICES *bs) {
	for (uint64_t addr = 0; addr < 4ULL * 0x40000000ULL; addr += 0x200000ULL) {
		map_address(bs, addr, addr);
	}
}

void setup_high_half_map(EFI_BOOT_SERVICES *bs) {
	uint64_t kernel_phys = 0x200000ULL;
	uint64_t kernel_virt = 0xFFFFFFFF80000000ULL;

	for (uint64_t offset = 0; offset < 4ULL * 0x100000ULL; offset += 0x200000ULL) {
		map_address(bs, kernel_phys + offset, kernel_virt + offset);
	}
}

void load_cr3() {
	uint64_t pml4_base = (uint64_t)pml4;

	__asm__ volatile (
			"mov %0, %%cr3"
			:
			: "r"(pml4_base)
			: "memory"
	);
}

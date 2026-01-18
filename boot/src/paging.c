#include "paging.h"

UINT64 *pml4 = NULL;

#define PAGE_PRESENT (1ULL << 0)
#define PAGE_RW      (1ULL << 1)
#define PAGE_PCD     (1ULL << 4)
#define PAGE_MASK    0x000FFFFFFFFFF000ULL

void *alloc_page(EFI_BOOT_SERVICES *bs) {
    EFI_PHYSICAL_ADDRESS addr;
    bs->AllocatePages(
            AllocateAnyPages,
            EfiLoaderData,
            1,
            &addr);

    return (void*)addr;
}

void map_address(EFI_BOOT_SERVICES *bs, UINT64 PA, UINT64 VA, UINT64 flags) {
	if (pml4 == NULL) {
		pml4 = alloc_page(bs);
		for (UINT64 i = 0; i < 512; i++)
			pml4[i] = 0;
	}

	UINT64 pml4_index = (VA >> 39) & 0x1FF;
	UINT64 pdpt_index = (VA >> 30) & 0x1FF;
	UINT64 pd_index   = (VA >> 21) & 0x1FF;
	UINT64 pt_index   = (VA >> 12) & 0x1FF;

	UINTN *pdpt = NULL;
	if (!(pml4[pml4_index] & PAGE_PRESENT)) {
		pml4[pml4_index] = (UINT64)alloc_page(bs) | PAGE_PRESENT | PAGE_RW;
	}
	pdpt = (UINT64*)(pml4[pml4_index] & PAGE_MASK);

	UINT64 *pd = NULL;
	if (!(pdpt[pdpt_index] & PAGE_PRESENT)) {
		pdpt[pdpt_index] = (UINT64)alloc_page(bs) | PAGE_PRESENT | PAGE_RW;
	}
	pd = (UINT64*)(pdpt[pdpt_index] & PAGE_MASK);

	UINT64 *pt = NULL;
	if (!(pd[pd_index] & PAGE_PRESENT)) {
		pd[pd_index] = (UINT64)alloc_page(bs) | PAGE_PRESENT | PAGE_RW;
	}
	pt = (UINT64*)(pd[pd_index] & PAGE_MASK);

	if (!(pt[pt_index] & PAGE_PRESENT)) {
		pt[pt_index] = (PA & PAGE_MASK) | flags;
	}
}

void setup_identity_map(
		EFI_BOOT_SERVICES *bs,
		EFI_MEMORY_DESCRIPTOR *mem_map,
		UINTN mem_map_size,
		UINTN desc_size) {
	for (UINTN i = 0; i < mem_map_size / desc_size; i++) {
		EFI_PHYSICAL_ADDRESS start = mem_map[i].PhysicalStart;
		UINT64 flags = PAGE_RW | PAGE_PRESENT;

		if (mem_map[i].Type != EfiConventionalMemory &&
			mem_map[i].Type != EfiLoaderData &&
			mem_map[i].Type != EfiLoaderCode) { continue; }

		if (mem_map[i].Type == EfiMemoryMappedIO || mem_map[i].Type == EfiMemoryMappedIOPortSpace)
			flags |= PAGE_PCD;

		for (UINTN j = 0; j < mem_map[i].NumberOfPages; j++) {
			UINT64 pa = start + j * 0x1000ULL;
			map_address(bs, pa, pa, flags);
		}
	}
}

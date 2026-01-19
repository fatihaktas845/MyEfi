#include "early_paging.h"

#define PAGE_PRESENT (1ULL << 0)
#define PAGE_RW      (1ULL << 1)
#define PAGE_PS      (1ULL << 7)

#define PML4_INDEX(x) (((x) >> 39) & 0x1FF)
#define PDPT_INDEX(x) (((x) >> 30) & 0x1FF)
#define PD_INDEX(x)   (((x) >> 21) & 0x1FF)

#define HIGH_HALF 0xFFFFFFFF80000000ULL



__attribute__((section(".text.boot"), noinline, used))
void *memcpy(void *dst, const void *src, uint64_t n) {
    unsigned char *d = dst;
    const unsigned char *s = src;
    while (n--) *d++ = *s++;
    return dst;
}


__attribute__((section(".text.boot"), noinline, used))
void *memset(void *dst, int value, unsigned long n)
{
    unsigned char *p = (unsigned char *)dst;
    unsigned char v = (unsigned char)value;

    while (n--) {
        *p++ = v;
    }
    return dst;
}


__attribute__((section(".bss.boot"), aligned(0x1000), used))
uint64_t pml4[512];



__attribute__((section(".bss.boot"), aligned(0x1000), used))
static unsigned char early_mem[4 * 1024 * 1024];

#define PAGE_SIZE 4096

__attribute__((section(".bss.boot"), aligned(0x1000), used))
uint64_t early_alloc_ptr;

__attribute__((section(".text.boot"), noinline, used))
void early_page_alloc_init() {
    early_alloc_ptr = (uint64_t)early_mem;
}

__attribute__((section(".text.boot"), noinline, used))
void* early_page_alloc() {
    void *ret = (void*)early_alloc_ptr;
    early_alloc_ptr += PAGE_SIZE;
    return ret;
}



__attribute__((section(".text.boot"), noinline, used))
void map_addr(uint64_t PA, uint64_t VA) {
	uint64_t *pdpt;
	if (!(pml4[PML4_INDEX(VA)] & PAGE_PRESENT)) {
		pdpt = (uint64_t*)early_page_alloc();
		memset(pdpt, 0, 0x1000);
		pml4[PML4_INDEX(VA)] = (uint64_t)pdpt | PAGE_PRESENT | PAGE_RW;
	} else {
		pdpt = (uint64_t*)(pml4[PML4_INDEX(VA)] & ~0xFFFULL);
	}

	uint64_t *pd;
    if (!(pdpt[PDPT_INDEX(VA)] & PAGE_PRESENT)) {
        pd = (uint64_t*)early_page_alloc();
		memset(pd, 0, 0x1000);
		pdpt[PDPT_INDEX(VA)] = (uint64_t)pd | PAGE_PRESENT | PAGE_RW;
	} else {
		pd = (uint64_t*)(pdpt[PDPT_INDEX(VA)] & ~0xFFFULL);
	}

	pd[PD_INDEX(VA)] = PA | PAGE_PRESENT | PAGE_RW | PAGE_PS;
}




__attribute__((section(".text.boot"), noinline, used))
void setup_paging(BootInfo *bi) {
	early_page_alloc_init();

	EFI_MEMORY_DESCRIPTOR *mem_map = bi->kmmi->mem_map;
	uint64_t mem_map_size = bi->kmmi->mem_map_size;
	uint64_t desc_size = bi->kmmi->desc_size;

	//identity-map
	for (uint64_t i = 0; i < mem_map_size / desc_size; i++) {
		EFI_MEMORY_DESCRIPTOR *curr_desc =
    (EFI_MEMORY_DESCRIPTOR *)((uint8_t*)mem_map + i * desc_size);

		EFI_MEMORY_TYPE type = curr_desc->Type;

		if (type != EfiConventionalMemory &&
			type != EfiLoaderData &&
			type != EfiLoaderCode) continue;

		uint64_t desc_start = curr_desc->PhysicalStart & ~0x1FFFFFULL;
		uint64_t desc_end = curr_desc->PhysicalStart +
							curr_desc->NumberOfPages * 0x1000;
		for (uint64_t addr = desc_start; addr < desc_end; addr += 0x200000)
			map_addr(addr, addr);
	}

	// high-half-map
        for (uint64_t offset = 0; offset < 0xA00000; offset += 0x200000)
            map_addr(0x200000 + offset, HIGH_HALF + offset);

	uint64_t pml4_base = (uint64_t)&pml4;
	__asm__ volatile(
			"mov %0, %%cr3"
			:
			: "r"(pml4_base)
			: "memory"
	);
}

#include "early_paging.h"

#define PAGE_PRESENT (1ULL << 0)
#define PAGE_RW      (1ULL << 1)
#define PAGE_PS      (1ULL << 7)

#define PML4_INDEX(x) (((x) >> 39) & 0x1FF)

#define HIGH_HALF 0xFFFFFFFF80000000ULL

__attribute__((section(".bss.boot"), aligned(0x1000), used))
unsigned long long pml4[512];

__attribute__((section(".bss.boot"), aligned(0x1000), used))
unsigned long long pdpt_low[512];

__attribute__((section(".bss.boot"), aligned(0x1000), used))
unsigned long long pdpt_high[512];

__attribute__((section(".bss.boot"), aligned(0x1000), used))
unsigned long long pd_high[512];

__attribute__((section(".text.boot"), noinline, used))
void setup_paging() {
	pdpt_low[0] = 0x0ULL | PAGE_PRESENT | PAGE_RW | PAGE_PS;

	pd_high[0] = 0x00400000ULL | PAGE_PRESENT | PAGE_RW | PAGE_PS;
	pdpt_high[0] = (unsigned long long)pd_high | PAGE_PRESENT | PAGE_RW;

	pml4[0] = (unsigned long long)pdpt_low | PAGE_PRESENT | PAGE_RW;
	pml4[PML4_INDEX(HIGH_HALF)] = (unsigned long long)pdpt_high | PAGE_PRESENT | PAGE_RW;

	unsigned long long pml4_base = (unsigned long long)&pml4;
	__asm__ volatile(
			"mov %0, %%cr3"
			:
			: "r"(pml4_base)
			: "memory"
	);
}

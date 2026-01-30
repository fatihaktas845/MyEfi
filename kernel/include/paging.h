#ifndef PAGING_H
#define PAGING_H

#include "types.h"

#define PAGE_P  (1ULL << 0)
#define PAGE_RW (1ULL << 1)
#define PAGE_US (1ULL << 2)
#define PAGE_PS (1ULL << 7)

#define KERNEL_PA 0x200000ULL
#define KERNEL_VA 0xFFFFFFFF80000000ULL
#define HIGH_HALF_OFFSET (KERNEL_VA - KERNEL_PA)

void setPML4(uint64_t *PML4);
void initPML4();
void mapAddress(uint64_t PA, uint64_t VA, uint64_t flags);
void loadCR3();

void setupKernelMap();

#endif // PAGING_H

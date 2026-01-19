#ifndef __EARLY_PAGING_H__
#define __EARLY_PAGING_H__

#include "bootInfo.h"


void *memcpy(void *dst, const void *src, uint64_t n);
void early_page_alloc_init();
void *early_page_alloc();
void map_addr(uint64_t PA, uint64_t VA);
void setup_paging(BootInfo *bi);

#endif // __EARLY_PAGING_H__

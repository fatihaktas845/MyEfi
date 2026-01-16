#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "bootInfo.h"

#define NULL ((void*)0)

typedef struct free_block {
	uint32_t size;
	struct free_block* next;
} free_block_t;

free_block_t *freeList = NULL;

void initHeap(KernelHeapInfo *khi);

#endif // __MEMORY_H__

#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "bootInfo.h"

typedef struct block {
	struct block* next;
} block_t;

extern block_t *freeList;

void initPageAllocator();
void *allocPage();
void freePage(void *ptr);

#endif // __MEMORY_H__

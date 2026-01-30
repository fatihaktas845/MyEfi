#ifndef MEMORY_H
#define MEMORY_H

#include "bootInfo.h"

typedef struct block {
	struct block* next;
} block_t;

void initPageAllocator();
void *allocPage();
void freePage(void *ptr);

#endif // MEMORY_H

#include "memory.h"

block_t *freeList = NULL;

extern uint8_t __kernel_heap_start;

uint8_t *heap;

void initPageAllocator() {
	heap = &__kernel_heap_start;
	
    size_t pageCount = (4 * 1024 * 1024) / 4096;

	for (size_t i = 0; i < pageCount; i++) {
		block_t *page = (block_t *)(heap + i * 4096);
		page->next = freeList;
		freeList = page;
	}
}

void *allocPage() {
	if (!freeList)
		return NULL;
	
	block_t *new_page = freeList;
	freeList = new_page->next;

	return (void *)new_page;
}

void freePage(void *ptr) {
	if (!ptr)
		return;

	block_t *page = (block_t*)ptr;
	page->next = freeList;
	freeList = page;
}

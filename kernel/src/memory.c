#include "memory.h"

block_t *freeList = NULL;

extern uint8_t *__kernel_heap_start;

void initPageAllocator() {
    size_t pageCount = 16 * 1024 * 1024 / 4096;

	for (size_t i = 0; i < pageCount; i++) {
		block_t *page = (block_t*)(__kernel_heap_start + i * 4096);
		page->next = freeList;
		freeList = page;
	}
}

void *alloc_page() {
	if (!freeList)
		return NULL;
	
	block_t *new_page = freeList;
	freeList = new_page->next;

	return (void*)new_page;
}

void free_page(void *ptr) {
	if (!ptr)
		return;

	block_t *page = (block_t*)ptr;
	page->next = freeList;
	freeList = page;
}

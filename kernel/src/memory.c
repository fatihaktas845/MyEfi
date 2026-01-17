#include "memory.h"

void initPageAllocator(KernelHeapInfo *khi) {
    size_t pageCount = khi->heapSize / 4096;

	for (size_t i = 0; i < pageCount; i++) {
		block_t *page = (block_t*)((uint8_t*)khi->heapStart + i * 4096);
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

void free_page(void* ptr) {
	if (!ptr)
		return;

	block_t *page = (block_t*)ptr;
	page->next = freeList;
	freeList = page;
}

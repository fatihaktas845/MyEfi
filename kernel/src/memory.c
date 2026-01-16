#include "memory.h"

void initHeap(KernelHeapInfo *khi) {
    freeList = (free_block_t*)khi->heapStart;

    free_block_t *new_block = freeList;
    new_block->size = khi->heapSize - sizeof(free_block_t);
    new_block->next = NULL;
}

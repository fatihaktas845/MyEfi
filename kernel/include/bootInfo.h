#ifndef __BOOTINFO_H__
#define __BOOTINFO_H__

#include <efi.h>
#include "types.h"

typedef struct {
    uint32_t Width;
    uint32_t Height;
    uint32_t PixelsPerScanLine;
    uint64_t FrameBufferBase;
} KernelGOPInfo;

typedef struct {
    void     *heapStart;
    uint32_t heapSize;
} KernelHeapInfo;

typedef struct {
	EFI_MEMORY_DESCRIPTOR *mem_map;
	UINTN mem_map_size;
	UINTN desc_size;
} KernelMemMapInfo;

typedef struct {
    KernelGOPInfo    *kgi;
    KernelHeapInfo   *khi;
	KernelMemMapInfo *kmmi;
} BootInfo;

#endif // __BOOTINFO_H__

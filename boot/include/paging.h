#ifndef __PAGING_H__
#define __PAGING_H__

#include <efi.h>

extern UINT64 *pml4;

void *alloc_page(EFI_BOOT_SERVICES *bs);

void map_address(
		EFI_BOOT_SERVICES *bs,
		UINT64 PA,
		UINT64 VA,
		UINT64 flags);

void setup_identity_map(
		EFI_BOOT_SERVICES *bs,
		EFI_MEMORY_DESCRIPTOR *mem_map,
		UINTN mem_map_size,
		UINTN desc_size);

#endif // __PAGING_H__

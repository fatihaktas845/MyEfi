#ifndef __PAGING_H__
#define __PAGING_H__

#include <efi.h>

void *allocate_page(EFI_BOOT_SERVICES *bs);
void init_pml4(EFI_BOOT_SERVICES *bs);
void map_address(EFI_BOOT_SERVICES *bs, UINT64 pa, UINT64 va);

void setup_identity_map(
		EFI_BOOT_SERVICES *bs,
		EFI_MEMORY_DESCRIPTOR *mem_map,
		UINTN mem_map_size,
		UINTN desc_size);

void setup_high_half_map(EFI_BOOT_SERVICES *bs);

void load_cr3();

#endif // __PAGING_H__

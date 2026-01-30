#include "elf.h"
#include "paging.h"

void *memcpy(void *dest, void *src, uint64_t n) {
	uint8_t *d = (uint8_t*)dest;
	uint8_t *s = (uint8_t*)src;

	for (uint32_t i = 0; i < n; i++)
		d[i] = s[i];

	return dest;
}

void *memset(void *dest, uint8_t value, uint64_t n) {
	uint8_t *d = (uint8_t*)dest;

	for (uint32_t i = 0; i < n; i++)
		d[i] = value;

	return dest;
}

typedef struct {
	UINT32 Width;
	UINT32 Height;
	UINT32 PixelsPerScanLine;
	UINT64 FrameBufferBase;
} KernelGOPInfo;

typedef struct {
	EFI_MEMORY_DESCRIPTOR *mem_map;
	UINTN mem_map_size;
	UINTN desc_size;
} KernelMemMapInfo;

typedef struct {
	KernelGOPInfo    *kgi;
	KernelMemMapInfo *kmmi;
} BootInfo;




static inline void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile ( "outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}



void uint64_to_char16(UINT64 val, CHAR16* buf) {
    const CHAR16 hex[] = L"0123456789ABCDEF";
    int i;
    for (i = 0; i < 16; i++) {          // 64-bit = 16 hex hane
        buf[15 - i] = hex[val & 0xF];
        val >>= 4;
    }
    buf[16] = L'\0';                     // null-terminate
}


EFI_STATUS EFIAPI efiMain(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *ST) {
	EFI_BOOT_SERVICES *bs = ST->BootServices;

	ST->ConOut->ClearScreen(ST->ConOut);

	EFI_STATUS status = EFI_SUCCESS;
	EFI_LOADED_IMAGE_PROTOCOL *lip = NULL;
	EFI_GUID lip_guid = EFI_LOADED_IMAGE_PROTOCOL_GUID;
	
	bs->OpenProtocol(
			ImageHandle,
			&lip_guid,
			(VOID **)&lip,
			ImageHandle,
			NULL,
			EFI_OPEN_PROTOCOL_GET_PROTOCOL);

	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *sfsp = NULL;
	EFI_GUID sfsp_guid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;

	bs->OpenProtocol(
			lip->DeviceHandle,
			&sfsp_guid,
			(VOID **)&sfsp,
			ImageHandle,
			NULL,
			EFI_OPEN_PROTOCOL_GET_PROTOCOL);

	EFI_FILE_PROTOCOL *root = NULL;
	EFI_FILE_PROTOCOL *kernelFile = NULL;
	status = sfsp->OpenVolume(sfsp, &root);

	if (EFI_ERROR(status))
		ST->ConOut->OutputString(ST->ConOut, L"OpenVolume\r\n");

	status = root->Open(
			root,
			&kernelFile,
			L"\\EFI\\BOOT\\kernel.elf",
			EFI_FILE_MODE_READ,
			0);

	if (EFI_ERROR(status))
		ST->ConOut->OutputString(ST->ConOut, L"Open\r\n");

	Elf64_Ehdr *ehdr = NULL;
	UINTN ehdr_size = sizeof(Elf64_Ehdr);
	bs->AllocatePool(EfiLoaderData, ehdr_size, (VOID **)&ehdr);
	kernelFile->Read(kernelFile, &ehdr_size, (VOID *)ehdr);

	Elf64_Phdr *phdr_table = NULL;
	UINTN phdr_table_size = ehdr->e_phnum * ehdr->e_phentsize;
	bs->AllocatePool(EfiLoaderData, phdr_table_size, (VOID **)&phdr_table);
	kernelFile->SetPosition(kernelFile, ehdr->e_phoff);
	kernelFile->Read(kernelFile, &phdr_table_size, (VOID *)phdr_table);

	EFI_PHYSICAL_ADDRESS phys = 0x00200000ULL;
	EFI_PHYSICAL_ADDRESS virt = 0xFFFFFFFF80000000ULL;

	for (uint32_t i = 0; i < ehdr->e_phnum; i++) {
		if (phdr_table[i].p_type != PT_LOAD) continue;

		UINTN memsize = phdr_table[i].p_memsz;
		UINTN filesize = phdr_table[i].p_filesz;
		EFI_PHYSICAL_ADDRESS load_addr;

		load_addr = phys + (phdr_table[i].p_vaddr - virt);

		VOID *p_vaddr = (VOID *)(uintptr_t)load_addr;

		EFI_MEMORY_TYPE memType;
		memType = (phdr_table[i].p_flags & PF_X) ? EfiLoaderCode : EfiLoaderData;

		bs->AllocatePages(
				AllocateAddress,
				memType,
				EFI_SIZE_TO_PAGES(memsize),
				&load_addr);

		kernelFile->SetPosition(kernelFile, phdr_table[i].p_offset);
		kernelFile->Read(kernelFile, &filesize, p_vaddr);

		if (memsize > filesize) {
			UINT8 *bss_start = (UINT8*)p_vaddr + filesize;
			memset((VOID*)bss_start, 0, memsize - filesize);
		}
	}

	kernelFile->Close(kernelFile);
	root->Close(root);

	bs->CloseProtocol(
			lip->DeviceHandle,
			&sfsp_guid,
			ImageHandle,
			NULL);

	bs->CloseProtocol(
			ImageHandle,
			&lip_guid,
			ImageHandle,
			NULL);

	EFI_GRAPHICS_OUTPUT_PROTOCOL *gop = NULL;
	EFI_GUID gop_guid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	bs->LocateProtocol(&gop_guid, NULL, (VOID **)&gop);

	KernelGOPInfo *kgi = NULL;
	bs->AllocatePool(
			EfiLoaderData,
			sizeof(KernelGOPInfo),
			(VOID **)&kgi);

	kgi->PixelsPerScanLine = gop->Mode->Info->PixelsPerScanLine;
	kgi->FrameBufferBase = gop->Mode->FrameBufferBase;
	kgi->Width = gop->Mode->Info->HorizontalResolution;
	kgi->Height = gop->Mode->Info->VerticalResolution;

	BootInfo *bi = NULL;
	bs->AllocatePool(
			EfiLoaderData,
			sizeof(BootInfo),
			(VOID **)&bi);

	bi->kgi = kgi;

	uint64_t entry = (uint64_t)ehdr->e_entry;
	/* typedef VOID (*KERNEL_ENTRY)(BootInfo *bi);
	KERNEL_ENTRY kernel_entry = (KERNEL_ENTRY)entry; */

	UINTN MemMapSize;
	UINTN MapKey;
	UINTN DescriptorSize;
	UINT32 DescriptorVersion;
	EFI_MEMORY_DESCRIPTOR *MemMap = NULL;

	bs->GetMemoryMap(
			&MemMapSize,
			MemMap,
			&MapKey,
			&DescriptorSize,
			&DescriptorVersion);

	MemMapSize += 5 * DescriptorSize;
	bs->AllocatePool(EfiLoaderData, MemMapSize, (VOID **)&MemMap);

	bs->GetMemoryMap(
			&MemMapSize,
			MemMap,
			&MapKey,
			&DescriptorSize,
			&DescriptorVersion);

	KernelMemMapInfo* kmmi = NULL;
	bs->AllocatePool(
			EfiLoaderData,
			sizeof(KernelMemMapInfo),
			(VOID **)&kmmi);
	kmmi->mem_map = MemMap;
	kmmi->mem_map_size = MemMapSize;
	kmmi->desc_size = DescriptorSize;
	bi->kmmi = kmmi;

	init_pml4(bs);
	setup_identity_map(bs);
	setup_high_half_map(bs);
	

	bs->ExitBootServices(ImageHandle, MapKey);

	load_cr3();

	__asm__ volatile (
		"mov %0, %%rdi\n"
		"jmp *%1"
		:
		: "r"(bi), "r"(entry)
		: "rdi"
	);

	return EFI_SUCCESS;
}

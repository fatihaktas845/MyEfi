#include "paging.h"
#include "elf.h"

#define PT_LOAD 1

#define PF_X	0x1
#define PF_W	0x2
#define PF_R	0x4

void *memcpy(void *dest, void *src, uint64_t n) {
	uint8_t *d = (uint8_t*)dest;
	uint8_t *s = (uint8_t*)src;

	for (uint32_t i = 0; i < n; i++)
		d[i] = s[i];

	return dest;
}

void *memset(void *dest, uint64_t value, uint64_t n) {
	uint8_t *d = (uint8_t*)dest;
	uint8_t v = (uint8_t)value;

	for (uint32_t i = 0; i < n; i++)
		d[i] = v;

	return dest;
}

typedef struct {
	UINT32 Width;
	UINT32 Height;
	UINT32 PixelsPerScanLine;
	UINT64 FrameBufferBase;
} KernelGOPInfo;

typedef struct {
	VOID   *heapStart;
	UINT32 heapSize;
} KernelHeapInfo;

typedef struct {
	KernelGOPInfo  *kgi;
	KernelHeapInfo *khi;
} BootInfo;

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

	for (uint32_t i = 0; i < ehdr->e_phnum; i++) {
		UINTN memsize = phdr_table[i].p_memsz;
		UINTN filesize = phdr_table[i].p_filesz;
		EFI_PHYSICAL_ADDRESS vaddr = phdr_table[i].p_vaddr;
		VOID *p_vaddr = (VOID *)(uintptr_t)vaddr;

		EFI_MEMORY_TYPE memType;
		memType = (phdr_table[i].p_flags & PF_X) ? EfiLoaderCode : EfiLoaderData;

		bs->AllocatePages(
				AllocateAddress,
				memType,
				EFI_SIZE_TO_PAGES(memsize),
				&vaddr);
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

	KernelHeapInfo *khi = NULL;
	bs->AllocatePool(
			EfiLoaderData,
			sizeof(KernelHeapInfo),
			(VOID **)&khi);

	EFI_PHYSICAL_ADDRESS heap_start_addr;
	UINTN heap_size = 16*1024*1024;

	bs->AllocatePages(
			AllocateAnyPages,
			EfiLoaderData,
			EFI_SIZE_TO_PAGES(heap_size),
			&heap_start_addr);

	khi->heapStart = (VOID *)((uintptr_t)heap_start_addr);
	khi->heapSize = heap_size;

	BootInfo *bi = NULL;
	bs->AllocatePool(
			EfiLoaderData,
			sizeof(BootInfo),
			(VOID **)&bi);

	bi->kgi = kgi;
	bi->khi = khi;

	VOID *entry = (VOID *)(uintptr_t)ehdr->e_entry;
	typedef VOID (*KERNEL_ENTRY)(BootInfo *bi);
	KERNEL_ENTRY kernel_entry = (KERNEL_ENTRY)entry;

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

	setup_identity_map(bs, MemMap, MemMapSize, DescriptorSize);

	bs->ExitBootServices(ImageHandle, MapKey);

	kernel_entry(bi);

	return EFI_SUCCESS;
}

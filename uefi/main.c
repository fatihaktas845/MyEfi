#include <efi.h>

struct kernel_header {
	UINT64 entry_offset;
};

EFI_STATUS EFIAPI efiMain(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *ST) {
	EFI_BOOT_SERVICES *bs = ST->BootServices;

	ST->ConOut->ClearScreen(ST->ConOut);
	ST->ConOut->OutputString(ST->ConOut, L"Hello, World!\r\n");

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
			L"\\EFI\\BOOT\\kernel.bin",
			EFI_FILE_MODE_READ,
			0);

	if (EFI_ERROR(status))
		ST->ConOut->OutputString(ST->ConOut, L"Open\r\n");

	EFI_FILE_INFO *fi = NULL;
	EFI_GUID fi_guid = EFI_FILE_INFO_ID;
	UINTN bufferSize = 0;

	status = kernelFile->GetInfo(kernelFile, &fi_guid, &bufferSize, NULL);

	if (status != EFI_BUFFER_TOO_SMALL)
		ST->ConOut->OutputString(ST->ConOut, L"GetInfo 1\r\n");

	status = bs->AllocatePool(EfiBootServicesData, bufferSize, (VOID **)&fi);

	if (EFI_ERROR(status))
		ST->ConOut->OutputString(ST->ConOut, L"AllocatePool\r\n");

	status = kernelFile->GetInfo(
			kernelFile,
			&fi_guid,
			&bufferSize,
			fi);

	if (EFI_ERROR(status))
		ST->ConOut->OutputString(ST->ConOut, L"GetInfo 2\r\n");

	EFI_PHYSICAL_ADDRESS kernelAddress = 0;
	UINTN fileSize = fi->FileSize;
	
	UINTN header_size = sizeof(struct kernel_header);
	UINTN kernelSize = fileSize - header_size;
	struct kernel_header kheader;

	kernelFile->Read(
			kernelFile,
			&header_size,
			(VOID *)&kheader);

	status = bs->AllocatePages(
			AllocateAnyPages,
			EfiLoaderData,
			EFI_SIZE_TO_PAGES(kernelSize),
			&kernelAddress);

	if (EFI_ERROR(status))
		ST->ConOut->OutputString(ST->ConOut, L"AllocatePages\r\n");

	status = kernelFile->Read(
			kernelFile,
			&kernelSize,
			(VOID *)kernelAddress);

	if (EFI_ERROR(status))
		ST->ConOut->OutputString(ST->ConOut, L"Read\r\n");

	EFI_GRAPHICS_OUTPUT_PROTOCOL *gop = NULL;
	EFI_GUID gop_guid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;

	bs->LocateProtocol(&gop_guid, NULL, (VOID **)&gop);
	
	UINT32 pps = gop->Mode->Info->PixelsPerScanLine;
	UINT64 fb = gop->Mode->FrameBufferBase;


	typedef void (*KERNEL_ENTRY)(UINT32 *pps, UINT64 *fb);
	KERNEL_ENTRY kernelEntry = (KERNEL_ENTRY)(UINTN)(kernelAddress + kheader.entry_offset - header_size);

	ST->ConOut->OutputString(ST->ConOut, L"Jumping to kernel...\r\n");
	kernelEntry(&pps, &fb);
	ST->ConOut->OutputString(ST->ConOut, L"Returned to bootloader...\r\n");

	return EFI_SUCCESS;
}

#include "efi.h"

EFI_STATUS
EFIAPI
efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE  *SystemTable)
{
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Hello World!\n");
    while(1);
    return 0;
}

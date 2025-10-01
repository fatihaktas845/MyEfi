clang -target x86_64-unknown-windows -ffreestanding -fno-stack-protector -fshort-wchar -mno-red-zone -c uefi/main.c -o uefi/main.o -I ../gnu-efi/inc/

clang -target x86_64-unknown-windows -nostdlib -Wl,-entry:efiMain -Wl,-subsystem:efi_application uefi/main.o -o ../esp/EFI/BOOT/bootx64.efi

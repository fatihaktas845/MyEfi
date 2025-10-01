clang -target x86_64-unknown-elf -ffreestanding -mno-red-zone -fno-stack-protector -c kernel/kernel.c -o kernel/kernel.o -fPIE
ld.lld -nostdlib -e kmain -pie kernel/kernel.o -o ../esp/EFI/BOOT/kernel.elf

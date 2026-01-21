# Veyra-x86_64-UEFI
Veyra is a minimalist hobby operating system designed for UEFI-only 64-bit systems.
It features a custom bootloader, ELF64 kernel loader, and basic heap/stack management.
The system runs entirely on low-level code written in C and x86_64 Assembly.
## Features
- UEFI-only bootloader (boot.c)
- PE32+ → ELF64 ABI conversion
- Simple ELF64 parser and kernel loader
- High-Half mapped kernel
- Mix of Intel x86_64 Assembly and C for kernel entry (kernel_stub.asm)
## Requirements
- NASM ≥ 2.15
- Clang Compiler
- LLD Linker
- QEMU (or UEFI-capable physical system)
- Make
### Installing dependencies on Arch Linux
```
sudo pacman -S --needed nasm clang make qemu ovmf xorriso mtools
```
This installs NASM, Clang, linker tools, Make, QEMU emulator, and OVMF firmware for UEFI testing.
## Building
1. Clone the repository:
```
git clone https://github.com/fatihaktas845/Veyra.git
cd Veyra
```
2. Build the project using Make:
```
make
```
3. The output ISO will be:
`Veyra-x86_64-UEFI.iso`
## Running / Testing
### QEMU
```
qemu-system-x86_64 -bios /usr/share/ovmf/OVMF.fd -cdrom Veyra-x86_64-UEFI.iso
```
### Writing to a USB drive
```
sudo dd if=Veyra-x86_64-UEFI.iso of=/dev/sdX bs=4M status=progress
sync
```
Replace /dev/sdX with your USB device path.
## Roadmap / Future Plans
- Improve heap/free-list allocator
- Implement bitmap-based physical memory allocator
- Extend paging management and prepare for multitasking
- Enhance framebuffer functionality for graphics or terminal output
## License
MIT License © 2025 Fatih AKTAŞ
See LICENSE file for details.

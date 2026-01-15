# Veyra-x86_64-UEFI
Veyra is a minimalist hobby operating system designed for UEFI-only 64-bit systems.
It features a custom bootloader, ELF64 kernel loader, and basic heap/stack management.
The system runs entirely on low-level code written in C and x86_64 Assembly.
## Features
- UEFI-only bootloader (boot.c)
- PE32+ → ELF64 ABI conversion
- Simple ELF64 parser and kernel loader
- Kernel-side identity-mapped 1 GiB memory
- Fixed heap with a simple free-list allocator (planned)
- Minimal framebuffer usage (kernel currently paints the screen red)
- Mix of Intel x86_64 Assembly and C for kernel entry (kernel_stub.asm)
## Requirements
- NASM ≥ 2.15
- GCC and Binutils (x86_64 target)
- LD linker
- QEMU (or UEFI-capable physical system)
- Make\
### Installing dependencies on Arch Linux
`sudo pacman -S --needed nasm gcc binutils make qemu ovmf`
This installs NASM, GCC, linker tools, Make, QEMU emulator, and OVMF firmware for UEFI testing.
## Building
1. Clone the repository:
`git clone https://github.com/fatihaktas845/Veyra.git`
`cd Veyra`
2. Build the project using Make:
`make`
3. The output ISO will be:
`Veyra-x86_64-UEFI.iso`
## Running / Testing
### QEMU
`qemu-system-x86_64 -bios /usr/share/ovmf/OVMF.fd -cdrom Veyra-x86_64-UEFI.iso`
### Writing to a USB drive
```sudo dd if=Veyra-x86_64-UEFI.iso of=/dev/sdX bs=4M status=progress
sync```
Replace /dev/sdX with your USB device path.
## Directory Structure
Veyra/
├─ boot/
│  ├─ boot.c          # UEFI bootloader
│  └─ elf.h           # ELF64 parser header
"├─ kernel/\n"
"│  ├─ kernel.c        # Kernel main\n"
"│  ├─ kernel_stub.asm # Kernel entry assembly\n"
"│  └─ kernel.ld       # Linker script\n"
"├─ Makefile\n"
"└─ LICENSE\n\n"
"## Roadmap / Future Plans\n\n"
"- Improve heap/free-list allocator\n"
"- Implement bitmap-based physical memory allocator\n"
"- Extend paging management and prepare for multitasking\n"
"- Enhance framebuffer functionality for graphics or terminal output\n\n"
"## License\n\n"
"MIT License © 2025 Fatih AKTAŞ\n"
"See LICENSE file for details.\n\n"
"## Boot Banner Suggestion\n\n"
"A simple boot message can be displayed via framebuffer:\n\n"
"Welcome to Veyra!\n"
"Veyra x86_64 Kernel Loading...\n";

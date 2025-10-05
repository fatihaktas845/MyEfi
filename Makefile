CC := clang -ffreestanding -fno-stack-protector -mno-red-zone
LD := ld.lld -nostdlib -e _start -pie
ASM := nasm -f elf64

BOOT_SRC := $(wildcard boot/*.c)
BOOT_OBJ := $(patsubst boot/%.c, boot/%.o, $(BOOT_SRC))

KERNEL_SRC := $(wildcard kernel/*.c)
KERNEL_OBJ := $(patsubst kernel/%.c, kernel/%.o, $(KERNEL_SRC))

ASM_SRC := $(wildcard kernel/*.asm)
ASM_OBJ := $(patsubst kernel/%.asm, kernel/%.o, $(ASM_SRC))

BOOT_TARGET   := esp/EFI/BOOT/bootx64.efi
KERNEL_TARGET := esp/EFI/BOOT/kernel.elf
ISO_TARGET	  := FatOS-x86_64.iso

.PHONY: all clear

all: $(ISO_TARGET)
	qemu-system-x86_64 -bios boot/OVMF.fd \
		-cdrom $(ISO_TARGET) \
		-net none \
		-machine q35

$(ISO_TARGET): $(BOOT_TARGET) $(KERNEL_TARGET)
	dd if=/dev/zero of=esp/efi.img bs=1M count=1
	mformat -i esp/efi.img ::
	mmd -i esp/efi.img ::/EFI
	mmd -i esp/efi.img ::/EFI/BOOT
	mcopy -i esp/efi.img $(BOOT_TARGET) ::/EFI/BOOT
	mcopy -i esp/efi.img $(KERNEL_TARGET) ::/EFI/BOOT
	xorriso -as mkisofs \
		-no-emul-boot \
		-e efi.img \
		-o $(ISO_TARGET) esp/

$(BOOT_TARGET): $(BOOT_OBJ)
	$(CC) -target x86_64-unknown-windows \
		-Wl,-subsystem:efi_application \
		-Wl,-entry:efiMain \
		-nostdlib \
		$^ -o $@

boot/%.o: boot/%.c
	mkdir -p esp/EFI/BOOT
	$(CC) -target x86_64-unknown-windows \
		-fshort-wchar \
		-I ../gnu-efi/inc \
		-c $< -o $@

$(KERNEL_TARGET): $(KERNEL_OBJ) $(ASM_OBJ)
	$(LD) $^ -o $@

kernel/%.o: kernel/%.c
	$(CC) -target x86_64-unknown-elf \
		-fPIE \
		-c $< -o $@

kernel/%.o: kernel/%.asm
	$(ASM) $< -o $@

clear:
	rm -rf esp \
		$(ISO_TARGET) \
		kernel/*.o \
		boot/*.o

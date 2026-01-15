CC := clang -ffreestanding -fno-stack-protector -mno-red-zone
LD := ld.lld -nostdlib -no-pie

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

$(shell mkdir -p esp/EFI/BOOT)

all: $(ISO_TARGET)
	qemu-system-x86_64 -bios boot/OVMF.fd \
		-drive if=virtio,format=raw,file=$(ISO_TARGET) \
		-net none \
		-m 512M \
		-vnc :0 \
		-monitor stdio

$(ISO_TARGET): $(BOOT_TARGET) $(KERNEL_TARGET)
	dd if=/dev/zero of=esp/efi.img bs=1M count=64
	mkfs.vfat -F 32 esp/efi.img
	mmd -i esp/efi.img ::/EFI
	mmd -i esp/efi.img ::/EFI/BOOT
	mcopy -i esp/efi.img $(BOOT_TARGET) ::/EFI/BOOT
	mcopy -i esp/efi.img $(KERNEL_TARGET) ::/EFI/BOOT
	xorriso -as mkisofs \
		-no-emul-boot \
		-eltorito-alt-boot \
		-isohybrid-gpt-basdat \
		-e efi.img \
		-o $(ISO_TARGET) esp/

$(BOOT_TARGET): $(BOOT_OBJ)
	$(CC) -target x86_64-unknown-windows \
		-Wl,-subsystem:efi_application \
		-Wl,-entry:efiMain \
		-nostdlib \
		$^ -o $@

boot/%.o: boot/%.c
	$(CC) -target x86_64-unknown-windows \
		-fshort-wchar \
		-I ../gnu-efi/inc \
		-c $< -o $@

$(KERNEL_TARGET): $(KERNEL_OBJ) $(ASM_OBJ)
	$(LD) $^ -o $@ \
		-T kernel/kernel.ld \
		-z max-page-size=0x1000

kernel/%.o: kernel/%.c
	$(CC) -target x86_64-unknown-elf \
		-fno-pie \
		-c $< -o $@

kernel/%.o: kernel/%.asm
	nasm -f elf64 $< -o $@

clear:
	rm -rf esp \
		$(ISO_TARGET) \
		kernel/*.o \
		boot/*.o

CC = x86_64-w64-mingw32-gcc
CFLAGS  = -ffreestanding # -I ../gnu-efi/inc
LDFLAGS = -nostdlib -mno-red-zone -Wl,-dll -Wl,--subsystem,10 -shared -e EfiMain

SRC_FILES := $(wildcard *.c)
OBJ_FILES := $(SRC_FILES:.c=.o)

.PHONY: all clean qemu

all: bootx64.efi

bootx64.efi: $(OBJ_FILES)
	$(CC) $(LDFLAGS) $< -o $@
	cp bootx64.efi drive/EFI/BOOT

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f *.o bootx64.efi

qemu:
	qemu-system-x86_64 -bios /usr/share/ovmf/OVMF.fd -drive format=raw,file=fat:rw:drive/ -net none -nographic

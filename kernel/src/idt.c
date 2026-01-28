#include "idt.h"
#include "psf.h"

struct IDTEntry idtEntries[256];
struct IDTR idtPtr;

void setIDTEntry(uint16_t index, void (*func)(), uint8_t flags) {
	uint64_t offset = (uint64_t)func;

	idtEntries[index].offsetLow = offset & 0xFFFF;
	idtEntries[index].segmentSelector = 0x0008;
	idtEntries[index].ist = 0;
	idtEntries[index].flags = 0x8E;
	idtEntries[index].offsetMedium = (offset >> 16) & 0xFFFF;
	idtEntries[index].offsetHigh = (offset >> 32) & 0xFFFFFFFF;
	idtEntries[index].reserved = 0;
}

void initIDT() {
	setIDTEntry(0, &divideError, 0x8E);

	idtPtr.size = sizeof(idtEntries) - 1;
	idtPtr.base = (uint64_t)&idtEntries;
}

void divideError() {
	printk("Divide Error!", 0x00FF0000);

	while (1) {
		__asm__ volatile("hlt");
	}
}

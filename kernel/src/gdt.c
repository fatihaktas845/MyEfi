#include "gdt.h"

struct GDTEntry entries[3];
struct GDTPtr gdtPtr;

void setupGDTEntry(
        uint8_t  index,
        uint32_t base,
        uint32_t limit,
        uint8_t  access,
        uint8_t  flags) {
	entries[index].limitLow    = limit & 0xFFFF;
	entries[index].baseLow     = base  & 0xFFFF;
	entries[index].baseMedium  = (base >> 16) & 0xFF;
	entries[index].access      = access;
	entries[index].granularity = ((limit >> 16) & 0x0F) | ((flags << 4) & 0xF0);
	entries[index].baseHigh    = (base >> 24) & 0xFF;
}

void initGDT() {
	setupGDTEntry(0, 0, 0, 0, 0); // NULL Descriptor
	setupGDTEntry(1, 0, 0x0FFFFF, 0x9A, 0x0A); // Kernel Code Descriptor
	setupGDTEntry(2, 0, 0x0FFFFF, 0x92, 0x08); // Kernel Data Descriptor

	gdtPtr.limit = sizeof(entries) - 1;
	gdtPtr.base  = (uint64_t)&entries;
}

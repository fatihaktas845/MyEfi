#ifndef IDT_H
#define IDT_H

#include "types.h"

struct IDTR {
	uint16_t size;
	uint64_t base;
} __attribute__((packed));

struct IDTEntry {
	uint16_t offsetLow;
	uint16_t segmentSelector;
	uint8_t  ist;
	uint8_t  flags;
	uint16_t offsetMedium;
	uint32_t offsetHigh;
	uint32_t reserved;
} __attribute__((packed));

void setIDTEntry(uint16_t index, void (*func)(), uint8_t flags);
void initIDT();

void divideError();

#endif // IDT_H

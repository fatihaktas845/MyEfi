#ifndef __GDT_H__
#define __GDT_H__

#include "types.h"

struct GDTEntry {
	uint16_t limitLow;
	uint16_t baseLow;
	uint8_t  baseMedium;
	uint8_t  access;
	uint8_t  granularity;
	uint8_t  baseHigh;
} __attribute((packed));

struct GDTPtr {
	uint16_t limit;
	uint64_t base;
} __attribute__((packed));

void setupGDTEntry(
		uint8_t  index,
		uint32_t base,
		uint32_t limit,
		uint8_t  access,
		uint8_t  flags);

void initGDT();

#endif // __GDT_H__

#ifndef PSF_H
#define PSF_H

#include "types.h"

typedef struct {
	uint8_t magic[2];
	uint8_t mode;
	uint8_t charsize;
} PSFv1_HEADER;

void initPSFv1();

void drawChar(
		uint8_t ascii,
		uint32_t x,
		uint32_t y,
		volatile uint32_t *fbb,
		uint32_t pps,
		uint8_t scale);

#endif // PSF_H

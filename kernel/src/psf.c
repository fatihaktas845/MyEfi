#include "psf.h"
#include "lat1_08.h"

PSFv1_HEADER *header;
uint8_t *glyphStart;

volatile uint32_t *fbb;
uint32_t pps;

void initPSFv1(volatile uint32_t *Fbb, uint32_t Pps) {
	header = (PSFv1_HEADER*)lat1_08_psf;
	glyphStart = (uint8_t*)header + sizeof(PSFv1_HEADER);

	fbb = Fbb;
	pps = Pps;
}

void drawChar(
		uint8_t ascii,
		uint32_t x,
		uint32_t y,
		uint8_t scale,
		uint32_t color) {
	uint8_t *glyph = glyphStart + ascii * header->charsize;

	for (uint32_t row = 0; row < header->charsize; row++) {
		uint8_t bits = glyph[row];

		for (uint8_t col = 0; col < 8; col++) {
            if (bits & (0x80 >> col)) {
                for (uint32_t sy = 0; sy < scale; sy++) {
                    for (uint32_t sx = 0; sx < scale; sx++) {
                        uint32_t pX = x + col * scale + sx;
                        uint32_t pY = y + row * scale + sy;
                        fbb[pY * pps + pX] = color;
                    }
                }
            }
		}
	}
}

void printString(
		const char *str,
		uint32_t x,
		uint32_t y,
		uint8_t scale,
		uint32_t color) {
	uint32_t newX = x;
	for (uint32_t i = 0; str[i]; i++) {
		drawChar(str[i], newX, y, scale, color);
		newX += 8 * scale;
	}
}

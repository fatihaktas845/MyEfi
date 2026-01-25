#include "psf.h"
#include "lat1_08.h"

PSFv1_HEADER *header;

uint8_t *glyphStart;

void initPSFv1() {
	header = (PSFv1_HEADER*)lat1_08_psf;
	glyphStart = (uint8_t*)header + sizeof(PSFv1_HEADER);
}

void drawChar(
		uint8_t ascii,
		uint32_t x,
		uint32_t y,
		volatile uint32_t *fbb,
		uint32_t pps,
		uint8_t scale) {
	uint8_t *glyph = glyphStart + ascii * header->charsize;

	for (uint32_t row = 0; row < header->charsize; row++) {
		uint8_t bits = glyph[row];

		for (uint8_t col = 0; col < 8; col++) {
            if (bits & (0x80 >> col)) {
                for (uint32_t sy = 0; sy < scale; sy++) {
                    for (uint32_t sx = 0; sx < scale; sx++) {
                        uint32_t pX = x + col * scale + sx;
                        uint32_t pY = y + row * scale + sy;
                        fbb[pY * pps + pX] = 0x00FF0000;
                    }
                }
            }
		}
	}
}

#include "utils.h"

void memset(void *dest, uint8_t value, size_t n) {
	uint8_t *_dest = (uint8_t *)dest;

	for (size_t i = 0; i < n; i++)
		_dest[i] = value;
}

void memcpy(void *dest, void *src, size_t n) {
	uint8_t *_dest = (uint8_t *)dest;
	uint8_t *_src  = (uint8_t *)src;

	for (size_t i = 0; i < n; i++) {
		_dest[i] = _src[i];
	}
}

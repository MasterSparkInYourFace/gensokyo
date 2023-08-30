#include "util.h"

#define XLT_SIZE 103
char xlt[XLT_SIZE] = {
	 255,  255,  255,  255,  255,  255,  255,  255,
	 255,  255,  255,  255,  255,  255,  255,  255,
	 255,  255,  255,  255,  255,  255,  255,  255,
	 255,  255,  255,  255,  255,  255,  255,  255,
	 255,  255,  255,  255,  255,  255,  255,  255,
	 255,  255,  255,  255,  255,  255,  255,  255,
	 0,    1,    2,    3,    4,    5,    6,    7,
	 8,    9,    255,  255,  255,  255,  255,  255,
	 255,  10,   11,   12,   13,   14,   15,   255,
	 255,  255,  255,  255,  255,  255,  255,  255,
	 255,  255,  255,  255,  255,  255,  255,  255,
	 255,  255,  255,  255,  255,  255,  255,  255,
	 255,  10,   11,   12,   13,   14,   15
};

uint8_t x2i(char c) {
	if ((int8_t) c < 0 || (int8_t) c >= XLT_SIZE)
		return 255;
	return xlt[(int8_t) c];
}

// decided to become IOCCC programmer 'cause branches are expensive :3
// also I'm sure assuming encoding is very smart and portable
// (returns 0 if n is >= 15), otherwise lowercase hex
char i2x(uint8_t n) {
	// adding 49 gets us from '0' to 'a'
	return (n % 10 + '0' + n / 10 * 49) & ((n < 16) * 0xFF);
}

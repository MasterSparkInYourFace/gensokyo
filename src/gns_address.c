#include "gns_address.h"

#include <stdio.h> // temporary - debugging
#include <string.h>

// I should probably put these in a generic utils header at some point
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

static inline uint8_t x2i(char x) {
	if (x < 0 || x >= XLT_SIZE)
		return 255;
	return xlt[(int8_t) x];
}

static inline uint16_t sswap(uint16_t n, uint8_t cl) {
	switch (cl) {
		case 1:
			return n;
		case 2:
			return ((n >> 4) & 0xF) | ((n & 0xF) << 4);
		case 3:
			return ((n >> 8) & 0xF) | (n & 0xF0) | ((n & 0xF) << 8);
		case 4:
			return
				(((n >> 12) & 0xF)      ) |
				(((n >>  8) & 0xF) <<  4) |
				(((n >>  4) & 0xF) <<  8) |
				(((n      ) & 0xF) << 12);
	}
	return 0;
}

// copy section from uint16_t to address bytes
static inline int scopy(GensokyoAddr *addr, uint32_t p, uint16_t s) {
	// GNS_ADDR_SIZE - 1 means there's an extra byte somewhere somehow. very bad
	if (p >= GNS_ADDR_SIZE - 1)
		return GNSA_ERR_MAL_LEN;
	addr->gnsa_addr[p] = (s >> 8) & 0xFF;
	addr->gnsa_addr[p + 1] = s & 0xFF;
	return 0;
}

#define ADDR_SEP '~'
#define ADDR_SEP2 "~~"

int gns_addr_atob(GensokyoAddr *addr, const char *src) {
	char *end, *p2_begin, *c;
	int r;
	uint8_t xc, has_p2 = 1, finalized = 0;
	uint16_t sec = 0;
	// p2_pos is 14 because phase 2 travels backwards
	uint32_t p1_pos = 0, p2_pos = 14, secl = 0;
	
	memset(addr->gnsa_addr, 0, GNS_ADDR_SIZE);
	
	// pointer to end of phase 1 parsing section
	end = strstr(src, ADDR_SEP2);
	if (end == NULL) {
		has_p2 = 0;
		end = (char *) src + strlen(src);
	}

	// parse phase 1 = before the double-tilde
	for (c = (char *) src; c < end; c++) {
		if (*c != ADDR_SEP) {
			finalized = 0;
			secl++;
			if (secl > 4)
				return GNSA_ERR_MAL_SECLEN;
			if ((xc = x2i(*c)) == 255)
				return GNSA_ERR_MAL_SECINVAL;
			sec = (sec << 4) | (xc & 0xF);
		} else {
			if ((r = scopy(addr, p1_pos, sec)) < 0)
				return r;
			p1_pos += 2, secl = 0, sec = 0, finalized = 1;
		}
	}
	// DISGUSTING
	if (!finalized && (r = scopy(addr, (p1_pos += 2) - 2, sec)) < 0)
		return r;
	if (!has_p2)
		return 0;

	// parse phase 2 = after the double-tilde (parse in reverse)
	p2_begin = end + 2;
	end = (char *) src + strlen(src) - 1;
	secl = 0, sec = 0;
	for (c = end; c >= p2_begin; c--) {
		if (*c != ADDR_SEP) {
			finalized = 0;
			secl++;
			if (secl > 4)
				return GNSA_ERR_MAL_SECLEN;
			if ((xc = x2i(*c)) == 255)
				return GNSA_ERR_MAL_SECINVAL;
			sec = (sec << 4) | (xc & 0xF);
		} else {
			if (finalized || c == p2_begin)
				return GNSA_ERR_MAL_SECTILDE;
			if (p2_pos <= p1_pos) // overlap - phase 2 too long
				return GNSA_ERR_MAL_LEN;
			if ((r = scopy(addr, p2_pos, sswap(sec, secl))) < 0)
				return r;
			p2_pos -= 2, secl = 0, sec = 0, finalized = 1;
		}
	}
	if (!finalized && (r = scopy(addr, (p2_pos -= 2) + 2, sswap(sec, secl))) < 0)
		return r;
	// congrats on your fresh address in raw form
	return 0;
}

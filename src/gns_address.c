#include "gns_address.h"

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
	if ((int8_t) x < 0 || (int8_t) x >= XLT_SIZE)
		return 255;
	return xlt[(int8_t) x];
}

// decided to become IOCCC programmer 'cause branches are expensive :3
// also I'm sure assuming encoding is very smart and portable
static inline char i2x(uint8_t n) {
	// adding 49 gets us from '0' to 'a'
	return (n % 10 + '0' + n / 10 * 49) | ((n > 15) * 0xFF);
}

static inline uint8_t ex4(uint16_t s, uint8_t i) {
	return ((s >> (i << 2)) & 0xF) | ((i > 3) * 0xFF);
}

static inline uint16_t sswap(uint16_t n, uint8_t cl) {
	return
		(ex4(n, 3)      |
		 ex4(n, 2) << 4 |
		 ex4(n, 1) << 8 |
		 (n & 0xF) << 12) >> ((4 - cl) << 2);
}

// copy section from uint16_t to address bytes
static inline int scopy(GensokyoAddr *addr, uint32_t p, uint16_t s) {
	// GNS_ADDR_SIZE - 1 means there's an extra byte somewhere somehow. very bad
	if (p >= GNS_ADDR_SIZE - 1)
		return GNSA_ERR_LEN;
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
	uint32_t p1_pos = 0, p2_pos = GNS_ADDR_SIZE - 2, secl = 0;
	
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
				return GNSA_ERR_LEN;
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

int gns_addr_btoa(GensokyoAddr *addr, char *dst, uint64_t len) {
	uint16_t s;
	uint64_t clen, pos;
	uint32_t s0_begin, s0_len, s0_l_begin, s0_l_len;
	uint8_t  spos, leading, n, s0;

	// locate longest section of 0s for shortening
	for (s0_l_len = 0, s0_len = 0, s0 = 0, pos = 0; pos < GNS_ADDR_SIZE; pos += 2) {
		s = (addr->gnsa_addr[pos]) << 8 | addr->gnsa_addr[pos + 1];
		if (s == 0) {
			if (s0) {
				s0_len += 2;
				continue;
			}
			s0_begin = pos;
			s0_len = 2;
			s0 = 1;
		} else {
			s0 = 0;
			if (s0_len <= s0_l_len)
				continue;
			s0_l_len = s0_len;
			s0_l_begin = s0_begin;
		}
	}
	if (s0_len > s0_l_len) {
		s0_l_len = s0_len;
		s0_l_begin = s0_begin;
	}

	// len - 5 is for safety. this loop writes at most 5 bytes in one iteration
	for (pos = 0, clen = 0; clen < len - 5 && pos < GNS_ADDR_SIZE; pos += 2) {
		s = (addr->gnsa_addr[pos] << 8) | addr->gnsa_addr[pos + 1];
		if (!s && pos >= s0_l_begin && pos < s0_l_begin + s0_l_len) {
			dst[clen++] = ADDR_SEP;
			pos += s0_l_len - 2;
			continue;
		}
		for (spos = 0, leading = 1; spos < 4; spos++) {
			n = ex4(s, 3 - spos);
			if (n == 0 && leading)
				continue;
			leading = 0;
			dst[clen++] = i2x(n);
		}
		if (leading)
			dst[clen++] = '0';
		if (pos < GNS_ADDR_SIZE - 2)
			dst[clen++] = ADDR_SEP;
	}
	return (pos < GNS_ADDR_SIZE) * GNSA_ERR_LEN;
}

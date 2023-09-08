#include "gensokyo/util.h"

#include <stdlib.h>
#include <string.h>

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

inline static int maybejump(AAStream *aas, int c) {
	if (aas->aa_err_jump)
		longjmp(*aas->aa_err_jump_buf, c);
	return c;
}

// auto-alloc stream
inline static int increment_size(AAStream *aas, size_t ds) {
	uint8_t *tmp;
	if (aas->aa_pos + ds <= aas->aa_size)
		return 0;

	if (aas->aa_nblocks_max > 0 && (aas->aa_nblocks + 1) > aas->aa_nblocks_max)
		return -1; // running up against programmer-configured block limit
	tmp = realloc(aas->aa_data, aas->aa_size + aas->aa_block_size);
	if (!tmp)
		return -2; // realloc fail, old block left untouched
	memset(aas->aa_data + aas->aa_size, 0, aas->aa_block_size);
	aas->aa_data = tmp;
	aas->aa_size += aas->aa_block_size;
	aas->aa_nblocks++;
	return 0;
}

int aas_init(AAStream *aas, size_t block_size, size_t nblocks_max) {
	aas->aa_block_size = block_size;
	aas->aa_nblocks_max = nblocks_max;
	aas->aa_err_jump = 0;
	aas->aa_pos = 0;
	aas->aa_data = malloc(block_size);
	if (!aas->aa_data)
		return -1;
	memset(aas->aa_data, 0, block_size);
	aas->aa_size = block_size;
	aas->aa_nblocks = 1;
	return 0;
}

void aas_setjmp(AAStream *aas, jmp_buf *jmp, jmp_buf **old_jmp) {
	if (aas->aa_err_jump && old_jmp != NULL)
		*old_jmp = aas->aa_err_jump_buf;
	aas->aa_err_jump_buf = jmp;
	aas->aa_err_jump = (jmp == NULL) ? 0 : 1;
}

ssize_t aas_write(AAStream *aas, void *data, size_t data_size) {
	int r;
	size_t w;
	if ((r = increment_size(aas, data_size)) == -2)
		return maybejump(aas, -1); // alloc fail
	w = (r == 0) ? data_size : aas->aa_size - aas->aa_pos;
	if (!w)
		return 0;
	memcpy(aas->aa_data + aas->aa_pos, data, w);
	aas->aa_pos += w;
	return w;
}

ssize_t aas_write_byte(AAStream *aas, uint8_t byte) {
	int r;
	if ((r = increment_size(aas, 1)) < 0)
		return maybejump(aas, r + 1); // -2 => -1, -1 => 0
	aas->aa_data[aas->aa_pos++] = byte;
	return 1;
}

ssize_t aas_write_u32(AAStream *aas, uint32_t n) {
	int r;
	if ((r = increment_size(aas, 4)) < 0)
		return maybejump(aas, r); // it is an error if we can't fit the whole integer
	memcpy(aas->aa_data + aas->aa_pos, &n, 4);
	aas->aa_pos += 4;
	return 4;
}

ssize_t aas_write_u64(AAStream *aas, uint64_t n) {
	int r;
	if ((r = increment_size(aas, 8)) < 0)
		return maybejump(aas, r);
	memcpy(aas->aa_data + aas->aa_pos, &n, 8);
	aas->aa_pos += 8;
	return 8;
}

void aas_reuse(AAStream *aas) {
	if (!aas->aa_data)
		return;
	memset(aas->aa_data, 0, aas->aa_size);
	aas->aa_pos = 0;
}

void aas_free(AAStream *aas) {
	if (!aas->aa_data)
		free(aas->aa_data);
	aas->aa_data = NULL;
	aas->aa_pos = 0;
	aas->aa_size = 0;
}

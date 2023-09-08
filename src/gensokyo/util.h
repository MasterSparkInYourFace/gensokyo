#pragma once

#include <sys/types.h>
#include <stdint.h>
#include <setjmp.h>

// hex char to int and int to hex char
uint8_t x2i(char c);
char    i2x(uint8_t n);

// auto-allocating memory stream stuff
typedef struct AAStream_s {
	size_t    aa_block_size;
	size_t    aa_nblocks;
	size_t    aa_nblocks_max;
	size_t    aa_size;
	uint64_t  aa_pos;  // writing position
	uint8_t  *aa_data; // uint8_t for indexing capability
	jmp_buf  *aa_err_jump_buf;
	uint8_t   aa_err_jump; // whether or not to longjmp on error
} AAStream; // I was tempted to call it "ass-stream". this is essential information

// other operations (rewind, read etc.) can be performed without
// much work so routines for them would be useless

// for indefinite max size, use 0 for nblocks_max
int     aas_init(AAStream *aas, size_t block_size, size_t nblocks_max);
void	aas_setjmp(AAStream *aas, jmp_buf *jmp, jmp_buf **old_jmp);

ssize_t aas_write(AAStream *aas, void *data, size_t data_size);

ssize_t aas_write_byte(AAStream *aas, uint8_t byte);
ssize_t aas_write_u32(AAStream *aas, uint32_t n);
ssize_t aas_write_u64(AAStream *aas, uint64_t n);

void    aas_reuse(AAStream *aas); // zero out data and reset pos, but don't free data or reset size
void    aas_free(AAStream *aas);

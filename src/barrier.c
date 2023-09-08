#include "gensokyo/barrier.h"

#include <endian.h>
#include <string.h>

// I want to go home
int barrier_cons_header(GNSBarrierHeader *hdr, AAStream *aas) {
	jmp_buf b, *bo;
	if (hdr->magic != GNS_RMAGIC && hdr->magic != GNS_SMAGIC)
		return -1; // invalid magic number
	if (setjmp(b) != 0)
		return -2; // allocation failure
	aas_setjmp(aas, &b, &bo);
	aas_write_u32(aas, hdr->magic);
	aas_write_byte(aas, hdr->type);
	aas_write_u64(aas, htobe64(hdr->data_length));
	aas_setjmp(aas, bo, NULL);
	return 0;
}

int barrier_cons_auth(GNSBarrierAuth *auth, AAStream *aas) {
	jmp_buf b, *bo;
	if (setjmp(b) != 0)
		return -1;
	aas_setjmp(aas, &b, &bo);
	aas_write(aas, auth->user, GNS_AUTH_USER_SIZE);
	aas_write_byte(aas, auth->kex);
	aas_write_byte(aas, auth->cipher);
	aas_write_u64(aas, htobe64(auth->kex_size));
	aas_write_u64(aas, htobe64(auth->cipher_size));
	aas_setjmp(aas, bo, NULL);
	return 0;
}

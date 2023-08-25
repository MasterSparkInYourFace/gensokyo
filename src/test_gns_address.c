#include "gns_address.h"

#include <stdio.h>

void print_addr_err(int c) {
	switch (c) {
		case 0:
			puts("gns_addr_atob returned OK");
			break;
		case GNSA_ERR_MAL_SECLEN:
			fprintf(stderr, "gns_addr_atob: too many characters in section\n");
			break;
		case GNSA_ERR_MAL_SECINVAL:
			fprintf(stderr, "gns_addr_atob: invalid characters in address\n");
			break;
		case GNSA_ERR_MAL_SECTILDE:
			fprintf(stderr, "gns_addr_atob: invalid tilde placement\n");
			break;
		case GNSA_ERR_LEN:
			fprintf(stderr, "gns_addr_atob: address too long\n");
			break;
		default:
			fprintf(stderr, "gns_addr_atob: unknown error\n");
	}
}

void print_addr_btoa_err(int c) {
	switch (c) {
		case 0:
			puts("gns_addr_btoa returns OK");
			break;
		case GNSA_ERR_LEN:
			fprintf(stderr, "gns_addr_btoa: not enough space to write address\n");
			break;
		default:
			fprintf(stderr, "gns_addr_btoa: unknown error\n");
	}
}

void print_addr(GensokyoAddr *a) {
	for (uint32_t i = 0; i < GNS_ADDR_SIZE; i++)
		printf("%02hhx ", a->gnsa_addr[i]);
	puts("");
}

int main(int argc, char **args) {
	GensokyoAddr a;
	char ar[64] = { 0 };
	int r;

	if (argc < 2) {
		fprintf(stderr, "usage: %s <GensokyoNet address>\n", args[0]);
		return 1;
	}

	printf("address: %s\n", args[1]);
	r = gns_addr_atob(&a, args[1]);
	print_addr_err(r);
	if (r >= 0) {
		fputs("bytes: ", stdout);
		print_addr(&a);
	} else
		return -r;
	r = gns_addr_btoa(&a, ar, sizeof(ar) - 1);
	print_addr_btoa_err(r);
	if (r >= 0)
		printf("readable re-conversion: %s\n", ar);

	return -r;
}

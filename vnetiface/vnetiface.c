#ifdef WIN32
#error "imagine using winblows. get out of here"
#endif

#ifndef linux
#error "I'm only familiar with linux APIs. contributions for freebsd etc. are welcome"
#endif

#include "common/linux_util.h"
#include "common/os_net_util.h"
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <netinet/ether.h>

#define USAGE1 "usage: %s [-e <ether_addr>] [-h] create|destroy <interface>\n"
char *usage2 =
	"\t-e  Use ethernet address <ether_addr>\n"
	"\t-h  Display this message";

uint8_t parse_ether(char *src, uint8_t *dst) {
	struct ether_addr *a = ether_aton(src);
	if (a == NULL)
		return 0;
	memcpy(dst, a->ether_addr_octet, 6);
	return 1;
}

int main(int argc, char **argv) {
	uint8_t ether[6] = { 0x47, 0x45, 0x4E, 0x53, 0x4F, 0x4B };
	int modret;
	int opt;
	
	if (argc < 2) {
		fprintf(stderr, USAGE1, argv[0]);
		return 1;
	}
	
	if ((modret = hasmod("dummy")) < 0) {
		fprintf(stderr, "hasmod: %m\n");
		return 1;
	}

	if (!modret) {
		fputs("the 'dummy' kernel module needs to be loaded; run 'modprobe dummy' as root if on linux\n", stderr);
		return 1;
	}

	if (ensure_net_admin() < 1)
		return 1;

	opterr = 0;
	while ((opt = getopt(argc, argv, "e:h")) > -1) {
		switch (opt) {
			case 'e':
				if (!parse_ether(optarg, ether)) {
					fputs("invalid ether address\n", stderr);
					return 1;
				}
				break;
			case 'h':
				printf(USAGE1, argv[0]);
				puts(usage2);
				return 0;
			default:
				fprintf(stderr, "unknown option '%c'\n" USAGE1, optopt, argv[0]);
				return 1;
		}
	}
	// TODO: implement rtnetlink code for actually manipulating the interface

	return 0;
}

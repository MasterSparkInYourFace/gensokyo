#pragma once

#include <stdint.h>

// can't use enums because int

typedef enum BReqType_e {
	BREQ_CONNECT = 1,
	BREQ_DISCONNECT = 2,
	BREQ_MAX
} BReqType;

typedef struct BHeaderC_s {
	uint32_t magic;       // fourCC; "GnsC"
	uint8_t  type;        // type of message
	uint64_t data_length; // length of data following this header
} BHeaderC;

#ifdef unix
int barrier_connect_unix(char *path, uint8_t abstract);
#endif // unix

int barrier_connect_ip(char *addr, uint16_t port);
int barrier_connect_ip6(char *addr, uint16_t port);

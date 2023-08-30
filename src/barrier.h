#pragma once

#include <stdint.h>

// can't use enums because int

typedef enum GNSReqType_e {
	GNSR_CONNECT = 1,
	GNSR_DISCONNECT = 2,
	GNSR_RAW = 3,
	GNSR_ENC = 4,
	GNSR_MAX
} GNSReqType;

typedef struct GNSBarrierRequest_s {
	uint32_t magic;       // fourCC; "GnsR"
	uint8_t  type;        // type of message, from GNSReqType
	uint64_t data_length; // length of data following this header
} GNSBarrierRequest;

#ifdef unix
int barrier_connect_unix(char *path, uint8_t abstract);
#endif // unix

int barrier_connect_ip(char *addr, uint16_t port);
int barrier_connect_ip6(char *addr, uint16_t port);

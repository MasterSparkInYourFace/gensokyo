#pragma once

// can't wait for the publication of C23. typed enums are great,
// forced-int enums are not
#define GNSA_GLOBAL 1
#define GNSA_LOCAL  2
#define GNSA_HIDDEN 3

typedef struct GensokyoAddr_s {
	uint8_t gns_type; // GNSA_*
	uint8_t gns_prefix[7]; // network prefix
	uint8_t gns_unique[8]; // unique part
} GensokyoAddr;

#pragma once

#include <stdint.h>

// OR this with the message type for all encrypted messages
#define GNS_MSG_ENC 0x80

// good thing you don't have to assign enum values to
// an instance of the same enum type and can just cram
// them into a uint8_t if they fit
typedef enum GNSMsgType_e {
	GNS_REQ_CONNECT = 1,
	GNS_REQ_DISCONNECT,
	GNS_REQ_AUTH,
	GNS_REQ_MAX,

	GNS_RES_OK,
	GNS_RES_FAIL,
	GNS_RES_MAX
} GNSMsgType;

// note here: converting a string to bytes without reversal means big-endian
// endianness is worse than double negatives with the flip-flop thinking
#define GNS_RMAGIC 0x476E7352
#define GNS_SMAGIC 0x476E7353

typedef struct GNSBarrierHeader_s {
	uint32_t magic;       // "GnsR" for request, "GnsS" for response
	uint8_t  type;        // type of message -  from GNSMsgType
	uint64_t data_length; // length of data following this header
} GNSBarrierHeader;

// structs for aux data defined here
#define GNS_AUTH_USER_SIZE 62
#define GNS_AUTH_SIZE (GNS_AUTH_USER_SIZE + 18)
typedef struct GNSBarrierAuth_s {
	uint8_t  user[GNS_AUTH_USER_SIZE];
	uint8_t  kex;
	uint8_t  cipher;
	uint64_t kex_size;
	uint64_t cipher_size;
} GNSBarrierAuth;

// negotiation structures for authentication and encryption
// (used with GNS_REQ_CONNECT with data_length > 0)

// KEY EXCHANGE
#define GNS_KEX_X25519 1

// no need for a structure since X25519 doesn't have many
// negotiable variables. the key size is constant 256 bits.
// the size here is thus of the public key, which follows
// the auth header
#define GNS_KEX_X25519_SIZE 32

// ENCRYPTION
#define GNS_ENC_AES_GCM 1
#define GNS_ENC_AES_GCM_SIZE 1
typedef struct GNSBarrierAuth_AES_GCM_s {
	uint8_t  iv_len;     // should be 12 for AES_GCM
} GNSBarrierAuth_AES_GCM;

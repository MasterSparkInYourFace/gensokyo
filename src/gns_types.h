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
	GNS_REQ_MAX,

	GNS_RES_OK,
	GNS_RES_FAIL,
	GNS_RES_MAX
} GNSMsgType;

typedef struct GNSBarrierHeader_s {
	uint32_t magic;       // "GnsR" for request, "GnsS" for response
	uint8_t  type;        // type of message -  from GNSMsgType
	uint64_t data_length; // length of data following this header
} GNSBarrierHeader;

#define GNS_KEX_X25519  1
#define GNS_ENC_AES_GCM 1

// structs for aux data defined here
typedef struct GNSBarrierAuth_s {
	uint8_t  user[62];
	uint8_t  kex;
	uint8_t  cipher;
	uint64_t kex_size;
	uint64_t cipher_size;
} GNSBarrierAuth;

// TODO: flesh out the data needed for key exchange.
// Should we negotiate a hash algorithm for key generation?
typedef struct GNSBarrierKex_X25519_s {
	uint64_t pk_len; // public key length
} GNSBarrierKex_X25519;
// I'm not very good at type names
typedef struct GNSBarrierEnc_AES_GCM_s {
	uint8_t  iv_len;     // any larger size would be silly for an IV
	uint64_t aad_len;    // auth data
	uint64_t cipher_len; // ciphertext
} GNSBarrierEnc_AES_GCM;

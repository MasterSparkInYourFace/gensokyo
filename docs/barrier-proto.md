# Barrier (daemon) protocol
(subject to constant change)

All supported underlying protocols for communication are stream-based.
All values are transmitted in "network byte order".

Header structure for all messages regardless of direction:
```c
typedef struct GNSBarrierHeader_s {
	uint32_t magic;       // "GnsR" for request, "GnsS" for response
	uint8_t  type;        // type of message
	uint64_t data_length; // length of data following this header
} GNSBarrierHeader;
```

Encrypted messages have their `type` field's most significant bit set (ORed with `GNS_MSG_ENC`). The lower 7 bits represent the message type.

The structure for generic authentication requests is defined here:
```c
typedef struct GNSBarrierAuth_s {
	uint8_t  user[62];
	uint8_t  kex;    // GNS_KEX_X25519
	uint8_t  cipher; // GNS_CIPHER_AES_GCM
	uint64_t kex_size;    // negotiation data for key exchange
	uint64_t cipher_size; // negotiation data for encryption cipher
} GNSBarrierAuth;
```
The `user` field is always required and is the sole method by which non-anonymous network users are identified.
This header is followed by `kex_size` bytes of key exchange negotiation data, itself followed by `cipher_size` bytes of cipher negotiation data. The size of this data is NOT reported in the initial `GNSBarrierHeader`. **Encryption is detailed in another document (todo)**.
## Connection / handshake - client

A client first requests a connection to a barrier instance by sending it a message with the type of GNS_REQ_CONNECT. The magic number must be present and valid as in all requests, and is used as part of the client validation process.

No other data is necessary for a basic connection over any underlying protocol and as such the `data_length` field for those connections is 0. If authentication is required, it is set to the total size of the individual fields of the `GNSBarrierAuth` structure (worded this way because the size of the struct itself might be different depending on alignment, meanwhile the protocol does not align anything).

For authentication, the client picks a key exchange algorithm and encryption cipher, filling the `GNSBarrierAuth` structure accordingly before sending it and the appropriate data to the barrier **(todo; this data will be described in another document later)**.

**TODO: move this part to encryption docs. not relevant for the handshake itself**
When creating a user, the barrier operator may pick from a number of OpenSSL EVP key exchange methods supported by gensokyonet (at the time of writing, I only have plans for X25519).

Authentication also implies the establishment of an encrypted connection, the cipher of which is determined in this header and can be picked freely by the user (again, only one algorithm is actually planned for now, which is AES-GCM for its extra auth capability).

## Connection / handshake - barrier (todo)

When the barrier daemon gets hold of a new connection, the first message it receives must be the client handshake or else the connection is terminated.

Once a valid GNSR_CONNECT message
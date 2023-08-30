# Barrier (daemon) protocol
(todo)

All supported underlying protocols for communication are stream-based.
All values are transmitted in "network byte order".

Header structure for all requests from client to barrier daemon:
```c
typedef struct GNSBarrierRequest_s {
	uint32_t magic;       // fourCC; "GnsR"
	uint8_t  type;        // type of message
	uint64_t data_length; // length of data following this header
} GNSBarrierRequest;
```
Non-hardcoded request types, such as encrypted requests, with their own headers and data, are encapsulated with this header, as described below (todo).
## Connection / handshake (TODO)

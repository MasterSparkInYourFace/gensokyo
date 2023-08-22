# Barrier (daemon) protocol
(todo, blocked by address docs)

The barrier isn't concerned about connection IDs, as all supported underlying protocols for communication are stream-based.

All values are transmitted in "network byte order".
Header structure for all requests:
```c
typedef struct BHeaderC_s {
	uint32_t magic;       // fourCC; "GnsC"
	uint8_t  type;        // type of message
	uint64_t data_length; // length of data following this header
} BHeaderC;
```
## Connection / handshake (TODO)
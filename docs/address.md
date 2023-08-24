# GensokyoNet addresses

## Structure

An address in GensokyoNet is fairly simple. The components of the address are described by this structure:
```c
typedef struct GensokyoAddr_s {
	uint8_t gns_type; // address type
	uint8_t gns_prefix[7]; // network prefix
	uint8_t gns_unique[8]; // unique part
} GensokyoAddr;
```
The addresses bear a passing similarity to IPv6, with reduced complexity for the purposes of GensokyoNet.

## Types

- The first byte of the address holds its type. Currently it can be one of
	- `1 (GNSA_GLOBAL)` - A global address that supports address discovery and can be used in routing.
	- `2 (GNSA_LOCAL)` - A local address that supports discovery but not routing. Local addresses can send requests to addresses more than one hop away via a node with a GNSA_GLOBAL address, and that node can forward any responses to them (a-la real life).
	- `3 (GNSA_HIDDEN)` - Similar to GNSA_LOCAL, but cannot be discovered by adjacent nodes. The address will still be shown in requests.

## Human (and youkai)-readable representation

The string representation of GensokyoNet addresses is copied from the [Wikipedia article](https://en.wikipedia.org/wiki/IPv6_address#Representation) on IPv6 addresses. Why make new format when old format do trick?

I replaced colons with tildes as a fun way to distinguish between IPv6 addresses and GensokyoNet addresses.

Examples:
- `1ba~dcaf~e2fe~eda7~~1` - GNSA_GLOBAL address with a network prefix of `badcafe2feeda7` and a unique part of `0000000000000001`
- `1ba~dcaf~e2fe~eda7~1000~~` - same, except the unique part is `1000000000000000`
- `3af~~1` - GNSA_HIDDEN address with a prefix of `af00000000000000` and a unique part of `0000000000000001`

(god I don't look forward to writing the parser. spare the sympathy, text processing is my enemy)
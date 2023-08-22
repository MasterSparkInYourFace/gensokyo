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

The string representation of GensokyoNet addresses is again inspired by IPv6. It is a series of case-insensitive "sections" of hex characters separated by colons.

A colon in the address shifts the preceding bytes to the most significant available position.
Two colons in a row does the same and starts placing bytes relative to the end of the address.

Each address component is required to be delimited by a colon for readability, and additional sectioning between components is allowed.

Examples:
- `1:badcafe2feeda7:1` - GNSA_GLOBAL address with a network prefix of `badcafe2feeda7` and a unique part of `0000000000000001`
- `01:badcafe2feeda7:1:` - same, except the unique part is `0100000000000000`
- `01:badcafe2feeda7:10:` - ditto, except this time the unique part is `1000000000000000`
- `03:10::1` - GNSA_HIDDEN address with a prefix of `1000000000000000` and a unique part of `1`
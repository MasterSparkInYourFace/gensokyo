#pragma once

#include <stdint.h>

// can't wait for the publication of C23. typed enums are great,
// always-int enums are not. cpp macros are between the two.
// I wish rust was popular when I started systems programming so
// I would've picked a good toolset rather than C. now I'm stuck with C
// because rust is "too complex" for my addled brain to even read
// (nevermind the fact that I also write *perl*).
// we can't even have types that have specific widths, so you have to rely
// on a header that does a bunch of preprocessor checks in order to
// assign one of the built-in unclear-size int types to a defined size,
// which depends on the system you're compiling for.
// I've been writing C for like 6-7 years at this point and only now have I
// actually considered how ridiculous this is after getting to try out
// tools like Rust and Zig which have these types built into the language
// with handy names such as "u32" and "i8" (not "uint32_t" and "int8_t")
// and without the need to include a header that, while standard, isn't
// actually built into the compiler implementation.
// this rant brought to you by an extremely sleepy witch who's currently
// busy procrastinating from writing the address parser. you're welcome <3
#define GNSA_GLOBAL 1
#define GNSA_LOCAL  2
#define GNSA_HIDDEN 3

#define GNS_ADDR_SIZE 16 // I dare you to change this
typedef struct _sGensokyoAddr {
	uint8_t gnsa_addr[GNS_ADDR_SIZE];
} GensokyoAddr;

#define GNSA_ERR_MAL_SECLEN -1 // malformed address - too many characters in section
#define GNSA_ERR_MAL_SECINVAL -2 // malformed address - invalid character in section
#define GNSA_ERR_MAL_SECTILDE -3 // malformed address - second tilde pair
#define GNSA_ERR_LEN -4 // length-related error

int gns_addr_atob(GensokyoAddr *addr, const char *src);

// len: space available to write representation. I'd go with 64
int gns_addr_btoa(GensokyoAddr *addr, char *dst, uint64_t len);

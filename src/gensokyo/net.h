#pragma once
#include "gensokyo/types.h"
#include "gensokyo/util.h"

// socket helpers
#ifdef unix
#define UNF_ABSTRACT 1
#define UNF_BIND     2
int unix_socket(const char *addr, uint8_t flags);
#endif // unix

// port is in HOST BYTE ORDER. do not translate it yourself
#define TCPF_BIND 1
int tcp_ip_socket(const char *addr, uint16_t port, uint8_t flags);
int tcp_ip6_socket(const char *addr, uint16_t port, uint8_t flags);

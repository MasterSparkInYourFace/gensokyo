#pragma once
#include <stdint.h>

#ifdef unix
#define UF_ABSTRACT 1
#define UF_BIND     2
int unix_socket(const char *addr, uint8_t flags);
#endif

// port is in HOST BYTE ORDER. do not translate it yourself
#define TCPF_BIND 1
#define TCPF_IP6  2
int tcp_socket(const char *addr, uint16_t port, uint8_t flags);

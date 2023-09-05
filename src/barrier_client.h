#pragma once

#include "gns_types.h"

#ifdef unix
int barrier_connect_unix(char *path, uint8_t abstract);
#endif // unix

int barrier_connect_ip(char *addr, uint16_t port);
int barrier_connect_ip6(char *addr, uint16_t port);

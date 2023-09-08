#pragma once

#include "gensokyo/types.h"
#include "gensokyo/util.h"

// gensokyonet packet construction
int barrier_cons_header(GNSBarrierHeader *hdr, AAStream *aas);
int barrier_cons_auth(GNSBarrierAuth *auth, AAStream *aas);
int barrier_cons_auth_aes_gcm(GNSBarrierAuth_AES_GCM, AAStream *aas);

#ifdef unix
int barrier_connect_unix(char *path, uint8_t abstract);
#endif // unix

int barrier_connect_ip(char *addr, uint16_t port);
int barrier_connect_ip6(char *addr, uint16_t port);

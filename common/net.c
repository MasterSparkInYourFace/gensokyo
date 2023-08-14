#include "net.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#ifdef unix
#include <sys/un.h>

void unix_make_addr(struct sockaddr_un *sa, const char *addr, uint8_t abstract) {
	sa->sun_family = AF_UNIX;
	memset(sa->sun_path, 0, sizeof(sa->sun_path));
	strncpy(sa->sun_path + !!abstract, addr,
		sizeof(sa->sun_path) - (1 + !!abstract));
}

int unix_socket(const char *addr, uint8_t flags) {
	struct sockaddr_un sa;
	int (*s_func)(int, const struct sockaddr *, socklen_t) = (flags & UF_BIND) ? bind : connect;
	int s = socket(AF_UNIX, SOCK_STREAM, 0);
	if (s < 0)
		return -1;
	unix_make_addr(&sa, addr, flags & UF_ABSTRACT);
	if (s_func(s, (struct sockaddr *) &sa, sizeof(struct sockaddr_un)) < 0)
		return -2;
	return s;
}
#endif // unix

#define HAS_IP6 (flags & TCPF_IP6)
int tcp_socket(const char *addr, uint16_t port, uint8_t flags) {
	struct sockaddr_in  sa;
	struct sockaddr_in6 sa6;
	void *sap = HAS_IP6 ? (void *) &sa6.sin6_addr : (void *) &sa.sin_addr;
	int family = (flags & TCPF_IP6) ? AF_INET6 : AF_INET;
	int s;
	int (*s_func)(int, const struct sockaddr *, socklen_t) = (flags & TCPF_BIND) ? bind : connect;
	
	if (HAS_IP6 && !(flags & TCPF_BIND)) {
		// TODO: flow label drand48
	}

	if (inet_pton(family, addr, sap) < 1)
		return -1; // invalid address
	s = socket(family, SOCK_STREAM, 0);
	if (s < 0)
		return -2;
	return s;
}

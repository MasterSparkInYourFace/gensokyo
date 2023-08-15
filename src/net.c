#include "net.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#ifdef unix
#include <sys/un.h>

typedef int (*s_func)(int, const struct sockaddr *, socklen_t);

void unix_make_addr(struct sockaddr_un *sa, const char *addr, uint8_t abstract) {
	sa->sun_family = AF_UNIX;
	memset(sa->sun_path, 0, sizeof(sa->sun_path));
	strncpy(sa->sun_path + !!abstract, addr,
		sizeof(sa->sun_path) - (1 + !!abstract));
}

int unix_socket(const char *addr, uint8_t flags) {
	struct sockaddr_un sa;
	s_func sf = (flags & UNF_BIND) ? bind : connect;
	int s = socket(AF_UNIX, SOCK_STREAM, 0);
	if (s < 0)
		return -1;
	unix_make_addr(&sa, addr, flags & UNF_ABSTRACT);
	if (sf(s, (struct sockaddr *) &sa, sizeof(struct sockaddr_un)) < 0)
		return -2;
	return s;
}
#endif // unix

int ip_make_addr(struct sockaddr_in *sa, const char *addr, uint16_t port) {
	sa->sin_family = AF_INET;
	sa->sin_port = htons(port);
	if (inet_pton(AF_INET, addr, &sa->sin_addr) < 1)
		return -1; // invalid address
	return 0;
}

int ip6_make_addr(struct sockaddr_in6 *sa, const char *addr, uint16_t port, uint8_t genflow) {
	struct drand48_data rd;
	struct timespec t;
	long lr;

	sa->sin6_scope_id = 0;
	sa->sin6_family = AF_INET6;
	sa->sin6_port = htons(port);
	if (inet_pton(AF_INET6, addr, &sa->sin6_addr) < 1)
		return -1;
	if (!genflow)
		return 0;
	if (clock_gettime(CLOCK_REALTIME, &t) < 0)
		return -2; // this should never happen
	srand48_r(t.tv_nsec, &rd);
	lrand48_r(&rd, &lr);
	// more significant bits are better. flowinfo is 20-bit
	sa->sin6_flowinfo = (lr >> 11) & 0xFFFFF;
	return 0;
}

int tcp_ip_socket(const char *addr, uint16_t port, uint8_t flags) {
	s_func sf = (flags & TCPF_BIND) ? bind : connect;
	struct sockaddr_in sa;
	int s;
	
	if (ip_make_addr(&sa, addr, port) < 0)
		return -1;
	
	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s < 0)
		return -2;
	
	if (sf(s, (struct sockaddr *) &sa, sizeof(struct sockaddr_in)) < 0)
		return -3;

	return s;
}

int tcp_ip6_socket(const char *addr, uint16_t port, uint8_t flags) {
	struct sockaddr_in6 sa;
	int s, r;
	s_func sf = (flags & TCPF_BIND) ? bind : connect;

	if ((r = ip6_make_addr(&sa, addr, port, !(flags & TCPF_BIND))) < 0)
		return r;

	s = socket(AF_INET6, SOCK_STREAM, 0);
	if (s < 0)
		return -3;
	if (sf(s, (struct sockaddr *) &sa, sizeof(struct sockaddr_in6)) < 0)
		return -4;
	return s;
}

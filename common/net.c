#include "net.h"

#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

#ifdef linux

int rtnetlink_socket(Netlink *c, uint32_t mcast_group) {
	memset(&c->addr, 0, sizeof(struct sockaddr_nl));
	
	int s = socket(AF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE);

	if (s < 0)
		return -1; // see errno

	c->addr.nl_family = AF_NETLINK;
	c->addr.nl_pid    = getpid();
	c->addr.nl_groups = mcast_group;

	if (bind(s, (struct sockaddr *) &c->addr, sizeof(struct sockaddr_nl)) < 0)
		return -2; // errno again

	c->socket = s;
	c->netlink_sequence = 1;
	
	return s;
}

void netlink_close(Netlink *c) {
	if (!c->socket)
		return; // already closed
	
	close(c->socket);
	
	memset(c, 0, sizeof(Netlink));
}

#endif // linux

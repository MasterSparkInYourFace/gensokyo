#pragma once
#include <stdint.h>

#ifdef linux
#include <linux/if_link.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>

typedef struct Netlink_s {
	struct sockaddr_nl addr;
	int socket;
	uint32_t netlink_sequence;
} Netlink;

#define RTNETLINK_ATTR_BUF_SIZE 4096
typedef struct RTNL_LinkRequest_s {
	struct nlmsghdr header;
	struct ifinfomsg rt_info;
	uint8_t rt_attr_buf[RTNETLINK_ATTR_BUF_SIZE];
} RTNL_LinkRequest;

int  rtnetlink_socket(Netlink *c, uint32_t mcast_group);
void netlink_close(Netlink *c);

int rtnetlink_link_request(Netlink *c, struct nlmsghdr *hdr, uint16_t type);
#endif // linux

#ifdef unix
int unix_bind(const char *path);
int unix_connect(const char *path);
#endif

int tcp_bind_s(const char *addr, uint16_t port);
int tcp_bind_u32(uint32_t addr, uint16_t port);
int tcp_connect_s(const char *addr, uint16_t port);
int tcp_connect_u32(uint32_t addr, uint16_t port);

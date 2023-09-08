#include "gensokyo/net.h"

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

char *un_path = "/tmp/gensocket";
char *un_abs = "gensocket";

uint16_t tcp_port = 34524;

char *ip_addr = "127.0.0.1";
char *ip6_addr = "::1";

void print_unix_error(int c) {
	switch (c) {
		case -1:
			fprintf(stderr, "unix_socket: socket(): %m\n");
			break;
		case -2:
			fprintf(stderr, "unix_socket: bind(): %m\n");
			break;
	}
}

void print_ip_error(int c) {
	switch (c) {
		case -1:
			fputs("tcp_ip_socket: invalid IPv4 address\n", stderr);
			break;
		case -2:
			fprintf(stderr, "tcp_ip_socket: socket(): %m\n");
			break;
		case -3:
			fprintf(stderr, "tcp_ip_socket: bind(): %m\n");
			break;
	}
}

void print_ip6_error(int c) {
	switch (c) {
		case -1:
			fputs("tcp_ip6_socket: invalid IPv6 address\n", stderr);
			break;
		case -2:
			fprintf(stderr, "(WTF?) tcp_ip6_socket: clock_gettime(): %m\n");
			break;
		case -3:
			fprintf(stderr, "tcp_ip6_socket: socket(): %m\n");
			break;
		case -4:
			fprintf(stderr, "tcp_ip6_socket: bind(): %m\n");
			break;
	}
}

int main() {
	int s;
	
#ifdef unix
	printf("binding unix socket to %s\n", un_path);
	
	s = unix_socket(un_path, UNF_BIND);
	if (s < 0) {
		print_unix_error(s);
		return 1;
	}

	puts("unix bind OK");
	close(s);
	unlink(un_path);

	printf("binding abstract unix socket to %s\n", un_abs);

	s = unix_socket(un_path, UNF_BIND | UNF_ABSTRACT);
	if (s < 0) {
		print_unix_error(s);
		return 1;
	}

	puts("unix abstract bind OK");
	close(s);
#endif // unix

	printf("binding tcp/ip socket to %s\n", ip_addr);

	s = tcp_ip_socket(ip_addr, tcp_port, TCPF_BIND);
	if (s < 0) {
		print_ip_error(s);
		return 1;
	}

	puts("tcp/ip bind OK");
	close(s);

	printf("binding tcp/ipv6 socket to %s\n", ip6_addr);
	s = tcp_ip6_socket(ip6_addr, tcp_port, TCPF_BIND);
	if (s < 0) {
		print_ip6_error(s);
		return 1;
	}

	puts("tcp/ipv6 bind OK");
	return 0;
}

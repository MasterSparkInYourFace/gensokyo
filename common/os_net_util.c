#include "os_net_util.h"
#include <sys/capability.h>
#include <unistd.h>
#include <stdio.h>

int ensure_net_admin() {
	cap_flag_value_t net_admin;
	cap_value_t v_net_admin = CAP_NET_ADMIN;
	cap_t cap;

	if (geteuid() == 0)
		return 1;

	if (!CAP_IS_SUPPORTED(CAP_NET_ADMIN)) {
		fputs("CAP_NET_ADMIN not supported\n", stderr);
		return 0;
	}
	
	cap = cap_get_proc();
	if (cap == NULL) {
		fprintf(stderr, "cap_get_proc: %m\n");
		return -1;
	}
	if (cap_get_flag(cap, CAP_NET_ADMIN, CAP_EFFECTIVE, &net_admin) < 0) {
		fprintf(stderr, "cap_get_flag (1): %m\n");
		return -1;
	}

	if (net_admin == CAP_SET)
		return 1;

	if (cap_get_flag(cap, CAP_NET_ADMIN, CAP_PERMITTED, &net_admin) < 0) {
		fprintf(stderr, "cap_get_flag (2): %m\n");
		return -1;
	}
	if (net_admin != CAP_SET) {
		fputs("CAP_NET_ADMIN not enabled or permitted. enable it with capset(8). need at least permitted set, effective is optional\n", stderr);
		return 0;
	}
	net_admin = CAP_SET;
	if (cap_set_flag(cap, CAP_EFFECTIVE, 1, &v_net_admin, CAP_SET) < 0) {
		fprintf(stderr, "cap_set_flag: %m\n");
		return -1;
	}
	return 1;
}

#include "linux_util.h"
#include <string.h>

FILE *modopen() {
	return fopen("/proc/modules", "rb");
}

#define LL 1024
// if a line is longer than 1024 bytes (because of deps or the module name),
// I'm contacting the developers of that kernel module
int moditer(FILE *mod, char *dst, size_t len) {
	char line[LL];
	if (fgets(line, LL, mod) == NULL) {
		if (ferror(mod))
			return -1;
		return 0;
	}
	size_t cp = strchr(line, ' ') - line;
	strncpy(dst, line, cp < (len - 1) ? cp : len - 1);
	dst[cp] = 0;
	return 1;
}
#undef LL

int hasmod(const char *mod) {
	int modret;
	size_t modlen = strlen(mod) + 1;
	char smod[modlen];
	FILE *modf = modopen();
	
	if (modf == NULL)
		return -1;
	
	while ((modret = moditer(modf, smod, modlen)) > 0) {
		if (!strncmp(smod, mod, modlen - 1))
			return 1;
	}
	if (modret < 0)
		return -1;
	return 0;
}

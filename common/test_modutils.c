#include "linux_util.h"

// should print a list of module names

int main() {
	char mod[128];
	int modret;
	FILE *f = modopen();
	if (f == NULL) {
		fprintf(stderr, "modopen: %m\n");
		return 1;
	}
	while ((modret = moditer(f, mod, 128)) > 0)
		puts(mod);
	if (modret < 0) {
		fprintf(stderr, "moditer: %m\n");
		return 1;
	}
	fclose(f);
	modret = hasmod("xhci_pci");
	if (modret == -1) {
		fprintf(stderr, "hasmod: %m\n");
		return 1;
	}
	if (!modret)
		puts("xhci_pci not detected, you don't have USB 3 lmao. everyone clown on this computer");
	else
		puts("xhci_pci detected, USB 3 accepted");
	return 0;
}

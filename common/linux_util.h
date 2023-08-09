#pragma once

#include <stdio.h>

FILE *modopen();
int moditer(FILE *mod, char *dst, size_t len);
int hasmod(const char *mod);

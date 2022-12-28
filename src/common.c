#include "common.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void die() {
	if (getpid() == 1) {
		while (1)
			;
	} else {
		exit(1);
	}
}

char* strdupn(const char* str) {
	if (str[0] == '\0')
		return NULL;
	else
		return strdup(str);
}
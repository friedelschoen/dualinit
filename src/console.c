#include "console.h"

#include <fcntl.h>
#include <unistd.h>

void init_console() {
	int in	= open("/dev/console", O_RDONLY, 0);
	int out = open("/dev/console", O_RDWR, 0);
	dup2(in, 0);
	dup2(out, 1);
	dup2(out, 2);

	if (in > 2)
		close(in);
	if (out > 2)
		close(out);
}

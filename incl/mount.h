#pragma once

#include <stdbool.h>


typedef struct mount_option {
	const char* name;
	int			flags;
	bool		invert;
} mount_option_t;

extern const struct mount_option mount_options[];

int mount_flags(const char* options, const char** dest);
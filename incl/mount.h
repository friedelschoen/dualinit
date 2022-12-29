#pragma once

#include <stdbool.h>

typedef struct mount_option {
	const char* name;
	int			flags;
	bool		invert;
} mount_option_t;

/**
 * mount options (taken from util-linux)
 */
extern const mount_option_t mount_options[];

/**
 * parse mount flags to C-flags and remaining options
 */
int mount_flags(const char* options, const char** dest);
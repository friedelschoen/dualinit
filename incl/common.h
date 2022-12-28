#pragma once

#define INFO(format...)                      \
	{                                        \
		if (verbose && color) {              \
			printf("\e[36m::\e[0m " format); \
		} else if (verbose)                  \
			printf(":: " format);            \
	}

#define WARN(format...)                           \
	{                                             \
		if (color)                                \
			printf("\e[1;35mwarn\e[0m: " format); \
		else                                      \
			printf("error: " format);             \
	}

#ifdef IS_CLI
#	define DIE exit(1)
#else
#	define DIE   \
		while (1) \
			;
#endif

#define PANIC(format...)                           \
	{                                              \
		if (color)                                 \
			printf("\e[1;31merror\e[0m: " format); \
		else                                       \
			printf("error: " format);              \
		DIE;                                       \
	}

#define STRDUPN(s) \
	(*(s) != '\0' ? strdup(s) : NULL)

#define streq(a, b) \
	(strcmp((a), (b)) == 0)

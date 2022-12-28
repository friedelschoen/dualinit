#pragma once

#define VERBOSE(format...)                       \
	{                                            \
		if (color) {                             \
			printf("\e[33mdebug:\e[0m " format); \
		} else                                   \
			printf("debug: " format);            \
	}

#define INFO(format...)                      \
	{                                        \
		if (color) {                         \
			printf("\e[36m::\e[0m " format); \
		} else                               \
			printf(":: " format);            \
	}

#define WARN(format...)                           \
	{                                             \
		if (color)                                \
			printf("\e[1;35mwarn\e[0m: " format); \
		else                                      \
			printf("error: " format);             \
	}

#define DIE              \
	if (getpid() == 1) { \
		while (1)        \
			;            \
	} else {             \
		exit(1);         \
	}

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

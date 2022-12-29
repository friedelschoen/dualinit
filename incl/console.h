#pragma once

#include "common.h"

/**
 * prints debug info
 */
#define VERBOSE(format...)                   \
	{                                        \
		if (color) {                         \
			printf("\e[33m::\e[0m " format); \
		} else                               \
			printf("debug: " format);        \
	}

/**
 * prints regular info
 */
#define INFO(format...)                      \
	{                                        \
		if (color) {                         \
			printf("\e[36m::\e[0m " format); \
		} else                               \
			printf(":: " format);            \
	}

/**
 * prints a warning
 */
#define WARN(format...)                           \
	{                                             \
		if (color)                                \
			printf("\e[1;35mwarn\e[0m: " format); \
		else                                      \
			printf("warn: " format);              \
	}

/**
 * prints an error and dies
 */
#define PANIC(format...)                           \
	{                                              \
		if (color)                                 \
			printf("\e[1;31merror\e[0m: " format); \
		else                                       \
			printf("error: " format);              \
		die();                                     \
	}

/**
 * prints a warning with errno
 */
#define WARN_E(format, ...)                                                       \
	{                                                                             \
		if (color)                                                                \
			printf("\e[1;35mwarn\e[0m: " format, ##__VA_ARGS__, strerror(errno)); \
		else                                                                      \
			printf("warn: " format, ##__VA_ARGS__, strerror(errno));              \
	}

/**
 * prints an error and dies with errno
 */
#define PANIC_E(format, ...)                                                       \
	{                                                                              \
		if (color)                                                                 \
			printf("\e[1;31merror\e[0m: " format, ##__VA_ARGS__, strerror(errno)); \
		else                                                                       \
			printf("warn: " format, ##__VA_ARGS__, strerror(errno));               \
	}

/**
 * initiate the console (for pid == 1)
 */
void init_console();
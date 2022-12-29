#pragma once

#include "common.h"

/**
 * prints debug info
 */
#define VERBOSE(format...)                       \
	{                                            \
		if (color) {                             \
			printf("\e[33mdebug:\e[0m " format); \
		} else                                   \
			printf("debug: " format);            \
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
 * initiate the console (for pid == 1)
 */
void init_console();
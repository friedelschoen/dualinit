#pragma once

#include "common.h"

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
			printf("warn: " format);              \
	}

#define PANIC(format...)                           \
	{                                              \
		if (color)                                 \
			printf("\e[1;31merror\e[0m: " format); \
		else                                       \
			printf("error: " format);              \
		die();                                     \
	}


void init_console();
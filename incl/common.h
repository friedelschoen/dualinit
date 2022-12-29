#pragma once

/**
 * program dies, if pid == 1 the program hangs in a while(1)-loop otherwise exit(1)
 */
void die() __attribute__((noreturn));

/**
 * strdup but it returns NULL if `str` is empty
 */
char* strdupn(const char* str);

/**
 * just comparing strings
 */
#define streq(a, b) \
	(strcmp((a), (b)) == 0)

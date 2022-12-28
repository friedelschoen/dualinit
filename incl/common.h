#pragma once

void die() __attribute__((noreturn));

char* strdupn(const char* str);

#define streq(a, b) \
	(strcmp((a), (b)) == 0)

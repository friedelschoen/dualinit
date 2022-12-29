#pragma once

#include <stdbool.h>
#include <stdio.h>

#define SECTION_MAX		  20
#define SECTION_MOUNT_MAX 100
#define PATH_MAX		  200

typedef struct mount {
	const char* type;
	const char* source;
	const char* target;
	const char* options;
	int			flags;
	bool		try;
} mount_t;

typedef struct section {
	const char* name;
	const char* root;
	const char* init;
	mount_t		mounts[SECTION_MOUNT_MAX];
	int			mount_size;
} section_t;

typedef enum parse_error {
	P_ALLOC,		 // cannot allocate line
	P_IDENTIFIER,	 // invalid command
	P_USAGE,		 // invalid usage of command
	P_SCOPE,		 // invalid scope
	P_DATA,			 // parameter has invalid type
	P_SECTION,		 // no section defined
	P_REDEF,		 // init, master, default called more than once
} parse_error_t;


/**
 * defined sections
 */
extern section_t sections[];

/**
 * size of defined sections
 */
extern int section_size;

/**
 * if colored output is wished
 */
extern bool color;

/**
 * if debug messages should be printed
 */
extern bool verbose;

/**
 * defined global mounts
 */
extern mount_t mounts[];

/**
 * size of defined global mounts
 */
extern int mount_size;

/**
 * menu timeout (TODO)
 */
extern int timeout;

parse_error_t config_parsef(FILE* file, const char* filename);
parse_error_t config_parse(int fd, const char* filename);
void		  config_cleanup();
void		  config_reset();
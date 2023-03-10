#include "config.h"

#include "common.h"
#include "mount.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/mount.h>
#include <unistd.h>

#define CHECK_SECTION              \
	if (current_section == NULL) { \
		result = P_USAGE;          \
		goto error;                \
	}

#define CHECK_ROOT                 \
	if (current_section != NULL) { \
		result = P_USAGE;          \
		goto error;                \
	}

#define CHECK_PARAMS_EQUALS(n) \
	if (columns_size != (n)) { \
		result = P_USAGE;      \
		goto error;            \
	}

#define CHECK_PARAMS_BETWEEN(a, b)                  \
	if (columns_size < (a) || columns_size > (b)) { \
		result = P_USAGE;                           \
		goto error;                                 \
	}

#define CHECK_PARAMS_MORE(a)  \
	if (columns_size < (a)) { \
		result = P_USAGE;     \
		goto error;           \
	}

#define PARSE_BOOL(dest)                   \
	if (streq(columns[1], "true"))         \
		dest = true;                       \
	else if (streq(columns[1], "false")) { \
		dest = false;                      \
	} else {                               \
		result = P_DATA;                   \
		goto error;                        \
	}


section_t sections[SECTION_MAX];
mount_t	  mounts[SECTION_MOUNT_MAX];
int		  section_size = 0;
int		  mount_size   = 0;
bool	  color		   = false;
bool	  verbose	   = true;
int		  timeout	   = 10;


parse_error_t config_parse(int fd, const char* filename) {
	FILE* file = fdopen(fd, "r");
	return config_parsef(file, filename);
}

parse_error_t config_parsef(FILE* file, const char* filename) {
	section_t*	  current_section = NULL;
	bool		  in_mount		  = false;
	int			  linenr		  = 0;
	parse_error_t result		  = 0;
	int			  columns_size	  = 0;
	size_t		  alloc			  = 0;
	char*		  line_origin	  = NULL;
	ssize_t		  len			  = 0;
	ssize_t		  last_blank	  = 0;
	char*		  line			  = NULL;
	char		  columns[10][100];

	// `getline` fetches one line from `file`
	while ((len = getline(&line_origin, &alloc, file)) > 0) {
		linenr++;
		// as `getline` doesn't add a terminating `\0`, concatinate it
		if (len + 1 > (ssize_t) alloc) {
			line = realloc(line_origin, alloc = len + 1);
			if (line == NULL) {
				printf("error: cannot allocate line\n");
				result = P_ALLOC;
				goto error;
			}
			line_origin = line;
		} else {
			line = line_origin;
		}
		line[len] = '\0';

		last_blank = -1;

		// some truncating
		while (isblank(line[0]))
			line++, len--;

		for (ssize_t i = 0; i < len; i++) {
			if (isblank(line[i])) {
				if (last_blank == -1)
					last_blank = i;
			} else if (line[i] == '\n' || line[i] == ';' || line[i] == '#') {
				len = (last_blank != -1) ? last_blank : i;
				break;
			} else {
				last_blank = -1;
			}
		}

		// if it's an empty string, skip it
		if (len == 0)
			continue;

		// parse colums (aka. split by string)
		columns_size	  = 0;
		int	 column_index = 0;
		bool string		  = false;
		for (ssize_t i = 0; i <= len; i++) {
			if (i == len || (isblank(line[i]) && !string)) {
				if (column_index == 1 && columns[columns_size][0] == '-') {
					columns[columns_size][0] = '\0';
					columns_size++;
					column_index = 0;
				} else if (column_index > 0) {
					columns[columns_size][column_index] = '\0';
					columns_size++;
					column_index = 0;
				}
			} else if (line[i] == '"') {
				string = !string;
			} else {
				columns[columns_size][column_index++] = line[i];
			}
		}

		// end
		if (streq(columns[0], "end")) {
			CHECK_PARAMS_EQUALS(1);

			if (in_mount) {
				in_mount = false;
			} else if (current_section != NULL) {
				current_section = NULL;
			} else {
				result = P_SCOPE;
				goto error;
			}
			// <fstype> <source> <target> [options]
		} else if (in_mount) {
			CHECK_PARAMS_BETWEEN(3, 4);

			mount_t* mnt = (current_section != NULL)
							 ? &current_section->mounts[current_section->mount_size++]
							 : &mounts[mount_size++];

			mnt->try	= columns[0][0] == '*';
			mnt->type	= strdupn(mnt->try ? columns[0] + 1 : columns[0]);
			mnt->source = strdupn(columns[1]);
			mnt->target = strdupn(columns[2]);
			if (columns_size == 4) {
				mnt->flags = mount_flags(columns[3], &mnt->options);
			} else {
				mnt->flags	 = 0;
				mnt->options = NULL;
			}
			// mount
		} else if (streq(columns[0], "mount")) {
			CHECK_PARAMS_EQUALS(1);

			in_mount = true;
			// section
		} else if (streq(columns[0], "section")) {
			CHECK_ROOT;
			CHECK_PARAMS_EQUALS(3);

			if (current_section != NULL) {
				result = P_REDEF;
				goto error;
			}

			current_section				= &sections[section_size++];
			current_section->init		= NULL;
			current_section->mount_size = 0;
			current_section->name		= strdupn(columns[1]);
			current_section->root		= strdupn(columns[2]);
			// rshare/share <dirs...>
		} else if (streq(columns[0], "rshare") || streq(columns[0], "share")) {
			CHECK_PARAMS_MORE(2);

			for (int i = 1; i < columns_size; i++) {
				mount_t* mnt = (current_section != NULL)
								 ? &current_section->mounts[current_section->mount_size++]
								 : &mounts[mount_size++];

				char* target = columns[i];

				mnt->try = target[0] == '*';
				if (mnt->try)
					target++;
				mnt->source	 = strdupn(target);
				mnt->target	 = strdupn(target);
				mnt->type	 = NULL;
				mnt->options = NULL;
				mnt->flags	 = MS_BIND;
				if (columns[0][0] == 'r')	 // aka. equals rshare
					mnt->flags |= MS_REC;
			}
			// color <enable>
		} else if (streq(columns[0], "color")) {
			CHECK_ROOT;
			CHECK_PARAMS_EQUALS(2);

			PARSE_BOOL(color);
			// verbose <enable>
		} else if (streq(columns[0], "verbose")) {
			CHECK_ROOT;
			CHECK_PARAMS_EQUALS(2);

			PARSE_BOOL(verbose);
			// timeout <seconds>
		} else if (streq(columns[0], "timeout")) {
			CHECK_ROOT;
			CHECK_PARAMS_EQUALS(2);

			char* end;
			timeout = strtol(columns[1], &end, 10);
			if (end != strchr(columns[1], '\0')) {
				result = P_DATA;
				goto error;
			}
			// init <path>
		} else if (streq(columns[0], "init")) {
			CHECK_SECTION;
			CHECK_PARAMS_MORE(2);

			if (current_section->init != NULL) {
				result = P_REDEF;
				goto error;
			}

			current_section->init = strdupn(columns[1]);
			// include <file>
		} else if (streq(columns[0], "include")) {
			CHECK_ROOT;
			CHECK_PARAMS_EQUALS(2);

			int fd = open(columns[1], O_RDONLY | O_NONBLOCK);
			result = config_parse(fd, columns[1]);
			close(fd);
			if (result != 0)
				goto cleanup;
			// mmpf, unknown command
		} else {
			result = P_IDENTIFIER;
			goto error;
		}
	}

	// if there were no sections, raise an error as that not the intension
	if (section_size == 0) {
		result = P_SECTION;
		goto error;
	}

	// if you reach this, there were no errors
	// just skip to `cleanup`
	goto cleanup;

error:
	// pretty-print error
	printf("error in %s:%d: ", filename, linenr);
	switch (result) {
		case P_ALLOC:
			printf("cannot allocate line\n");
			break;
		case P_SECTION:
			printf("no section defined\n");
			break;
		case P_IDENTIFIER:
			printf("unknown identifier '%s'\n", columns[0]);
			break;
		case P_USAGE:
			printf("invalid usage of command '%s'\n", columns[0]);
			break;
		case P_SCOPE:
			printf("invalid scope of command '%s'\n", columns[0]);
			break;
		case P_DATA:
			printf("invalid paramter-type of '%s'\n", columns[0]);
			break;
		case P_REDEF:
			printf("redefinition of '%s'\n", columns[0]);
			break;
	}
	config_cleanup();

cleanup:
	if (line_origin != NULL)
		free(line_origin);

	return result;
}

void config_free_mount(mount_t* mount, int size) {
	for (int i = 0; i < size; i++) {
		if (mount[i].type != NULL)
			free((void*) mount[i].type);
		if (mount[i].source != NULL)
			free((void*) mount[i].source);
		if (mount[i].target != NULL)
			free((void*) mount[i].target);
		if (mount[i].options != NULL)
			free((void*) mount[i].options);
	}
}

void config_cleanup() {
	config_free_mount(mounts, mount_size);
	mount_size = 0;

	for (int i = 0; i < section_size; i++) {
		if (sections[i].name != NULL)
			free((void*) sections[i].name);
		if (sections[i].root != NULL)
			free((void*) sections[i].root);
		if (sections[i].init != NULL)
			free((void*) sections[i].init);
		config_free_mount(sections[i].mounts, sections[i].mount_size);
	}
	section_size = 0;
}

void config_reset() {
	config_cleanup();

	section_size = 0;
	mount_size	 = 0;
	color		 = false;
	verbose		 = true;
	timeout		 = 10;
}
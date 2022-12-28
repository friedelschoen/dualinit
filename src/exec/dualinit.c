#include "common.h"
#include "config.h"
#include "console.h"
#include "default.h"

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mount.h>
#include <unistd.h>


static void mount_chroot(const char* root, const mount_t* mnt) {
	static char dest[100];
	strcpy(dest, root);
	strcat(dest, mnt->target);

	INFO("mounting %s -> %s", mnt->source, dest);
	if (mnt->type != NULL)
		printf(" (%s)", mnt->type);
	if (mnt->options != NULL)
		printf(" [%s]", mnt->options);

	printf("\n");

	if (mount(mnt->source, dest, mnt->type, mnt->flags, mnt->options) != 0) {
		if (mnt->try) {
			WARN("mounting %s to %s failed: %s\n", mnt->source, dest, strerror(errno));
		} else {
			PANIC("mounting %s to %s failed: %s\n", mnt->source, dest, strerror(errno));
		}
	}
}

int main() {
	init_console();

	if (getpid() != 1) {
		PANIC("must run as PID 1\n");
	}

	int choice_file = open(DEFAULT_CONFIG, O_RDONLY | O_NONBLOCK);
	if (choice_file == -1) {
		PANIC("cannot open %s: %s\n", DEFAULT_CONFIG, strerror(errno));
	}

	parse_error_t parse_code = config_parse(choice_file, DEFAULT_CONFIG);
	if (parse_code != 0) {
		PANIC("invalid config");
	}
	close(choice_file);

	int section_index;
	while (1) {
		printf("which init do you want to start?\n");
		for (int i = 0; i < section_size; i++) {
			printf("[%d] %s at %s\n", i, sections[i].name, sections[i].root);
		}

		printf(": ");
		fflush(stdout);

		scanf("%d", &section_index);
		if (section_index >= 0 && section_index < section_size)
			break;

		WARN("your choice %d must be lower than %d\n\n", section_index, section_size);
	}

	section_t* section = &sections[section_index];
	bool	   is_root = strcmp(section->root, "/") == 0;

	if (!is_root) {
		mount_t self_mount = {
			.type	 = NULL,
			.source	 = section->root,
			.target	 = "/",
			.options = NULL,
			.flags	 = MS_BIND,
			.try	 = false,
		};

		mount_chroot(section->root, &self_mount);

		for (int i = 0; i < mount_size; i++) {
			mount_chroot(section->root, &mounts[i]);
		}
	}

	for (int i = 0; i < section->mount_size; i++) {
		mount_chroot(section->root, &section->mounts[i]);
	}

	if (!is_root) {
		INFO("chrooting into %s\n", section->root);

		if (chroot(section->root) == -1) {
			PANIC("cannot chroot into %s: %s\n", section->root, strerror(errno));
		}
	}

	if (chdir("/") == -1) {
		PANIC("error: cannot chdir into '/': %s\n", strerror(errno));
	}

	char init[PATH_MAX] = DEFAULT_INIT;
	if (section->init != NULL)
		strcpy(init, section->init);

	config_cleanup();

	INFO("entering %s\n\n", init);

	execlp(section->init, section->init, NULL);

	PANIC("error: cannot execute %s: %s\n", section->init, strerror(errno));
}

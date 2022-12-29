#include "mount.h"

#include <stdbool.h>
#include <string.h>
#include <sys/mount.h>


const mount_option_t mount_options[] = {
	{ "ro", MS_RDONLY, false },		/* read-only */
	{ "rw", MS_RDONLY, true },		/* read-write */
	{ "exec", MS_NOEXEC, true },	/* permit execution of binaries */
	{ "noexec", MS_NOEXEC, false }, /* don't execute binaries */
	{ "suid", MS_NOSUID, true },	/* honor suid executables */
	{ "nosuid", MS_NOSUID, false }, /* don't honor suid executables */
	{ "dev", MS_NODEV, true },		/* interpret device files  */
	{ "nodev", MS_NODEV, false },	/* don't interpret devices */

	{ "sync", MS_SYNCHRONOUS, false }, /* synchronous I/O */
	{ "async", MS_SYNCHRONOUS, true }, /* asynchronous I/O */

	{ "dirsync", MS_DIRSYNC, false },	  /* synchronous directory modifications */
	{ "remount", MS_REMOUNT, true },	  /* alter flags of mounted FS */
	{ "bind", MS_BIND, false },			  /* Remount part of the tree elsewhere */
	{ "rbind", MS_BIND | MS_REC, false }, /* Idem, plus mounted subtrees */
#ifdef MS_NOSUB
	{ "sub", MS_NOSUB, true },	  /* allow submounts */
	{ "nosub", MS_NOSUB, false }, /* don't allow submounts */
#endif
#ifdef MS_SILENT
	{ "silent", MS_SILENT, false }, /* be quiet  */
	{ "loud", MS_SILENT, true },	/* print out messages. */
#endif
#ifdef MS_MANDLOCK
	{ "mand", MS_MANDLOCK, false },	 /* Allow mandatory locks on this FS */
	{ "nomand", MS_MANDLOCK, true }, /* Forbid mandatory locks on this FS */
#endif
#ifdef MS_NOATIME
	{ "atime", MS_NOATIME, true },	  /* Update access time */
	{ "noatime", MS_NOATIME, false }, /* Do not update access time */
#endif
#ifdef MS_I_VERSION
	{ "iversion", MS_I_VERSION, false },  /* Update inode I_version time */
	{ "noiversion", MS_I_VERSION, true }, /* Don't update inode I_version time */
#endif
#ifdef MS_NODIRATIME
	{ "diratime", MS_NODIRATIME, true },	/* Update dir access times */
	{ "nodiratime", MS_NODIRATIME, false }, /* Do not update dir access times */
#endif
#ifdef MS_RELATIME
	{ "relatime", MS_RELATIME, false },	 /* Update access times relative to mtime/ctime */
	{ "norelatime", MS_RELATIME, true }, /* Update access time without regard to mtime/ctime */
#endif
#ifdef MS_STRICTATIME
	{ "strictatime", MS_STRICTATIME, false },  /* Strict atime semantics */
	{ "nostrictatime", MS_STRICTATIME, true }, /* kernel default atime */
#endif
#ifdef MS_LAZYTIME
	{ "lazytime", MS_LAZYTIME, false }, /* Update {a,m,c}time on the in-memory inode only */
	{ "nolazytime", MS_LAZYTIME, true },
#endif
#ifdef MS_NOSYMFOLLOW
	{ "symfollow", MS_NOSYMFOLLOW, true }, /* Don't follow symlinks */
	{ "nosymfollow", MS_NOSYMFOLLOW, false },
#endif
	{ 0 }
};

int mount_flags(const char* options, const char** dest_ptr) {
	int flags = 0;

	char option[20];
	char dest[strlen(options) + 1];
	int	 option_size = 0;
	int	 dest_size	 = 0;

	while (1) {
		if (*options == ',' || *options == '\0') {
			option[option_size] = '\0';

			for (const mount_option_t* flg = mount_options; flg->name != NULL; flg++) {
				if (strcmp(flg->name, option) == 0) {
					if (flg->invert)
						flags &= ~flg->flags;
					else
						flags |= flg->flags;
					option_size = 0;
					break;
				}
			}

			if (option_size > 0) {
				if (dest_size > 0)
					dest[dest_size++] = ',';
				memcpy(dest + dest_size, option, option_size);
				dest_size += option_size;
				option_size = 0;
			}

			if (*options == '\0')
				break;
		} else {
			option[option_size++] = *options;
		}

		options++;
	}

	if (dest_size > 0) {
		dest[dest_size] = '\0';
		*dest_ptr		= strdup(dest);
	} else {
		*dest_ptr = NULL;
	}

	return flags;
}
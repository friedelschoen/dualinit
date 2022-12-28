#include "default.h"

#include <sys/mount.h>

const mount_t DEFAULT_MOUNTS[] = {
	{ NULL, "/dev", "/dev", NULL, MS_BIND | MS_REC, false },
	// -			/dev		/dev		rbind
	{ NULL, "/", "/dualinit", NULL, MS_BIND, false },
	// -			/ 			/dualinit	bind
	{ "proc", "proc", "/proc", NULL, MS_RELATIME, false },
	// proc		proc		/proc		relatime
	{ "tmpfs", "run", "/run", "mode=0755", 0, false },
	// tmpfs		run			/run		mode=0755
	{ NULL, "/sys", "/sys", NULL, MS_BIND | MS_REC, false },
	// -			/sys		/sys		rbind
	{ "tmpfs", "tmp", "/tmp", "mode=1777", MS_STRICTATIME, false },
	// tmpfs		tmp			/tmp		mode=1777,strictatime
	{ 0 },
};

const char* DEFAULT_MASTER_MOUNTS[] = {
	"/boot", "/lost+found", NULL
};
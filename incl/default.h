#pragma once

#include "config.h"

#ifndef DEFAULT_CONFIG
#	define DEFAULT_CONFIG "/etc/dualinit.conf"
#endif

#ifndef DEFAULT_INIT
#	define DEFAULT_INIT "/sbin/init"
#endif

#ifndef REBOOT_INSTR
#	define REBOOT_INSTR "/etc/dualinit-reboot.txt"
#endif

#ifndef DEFAULT_EXEC_PATH
#	define DEFAULT_EXEC_PATH "/usr/share/dualinit/bin/init"
#endif

extern const mount_t DEFAULT_MOUNTS[];

extern const char* DEFAULT_MASTER_MOUNTS[];
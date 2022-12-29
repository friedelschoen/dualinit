#pragma once

#include "config.h"

/**
 * where the config lives
 */
#ifndef DEFAULT_CONFIG
#	define DEFAULT_CONFIG "/etc/dualinit.conf"
#endif

/**
 * default init-path if nothing is defined
 */
#ifndef DEFAULT_INIT
#	define DEFAULT_INIT "/sbin/init"
#endif
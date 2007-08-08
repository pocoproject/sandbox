#ifndef LIBSSH2_CONFIG_H
#define LIBSSH2_CONFIG_H

#ifdef _WIN32
#include "libssh2_config_win32.h"
#else
#include "libssh2_config_posix.h"
#endif

#endif /* LIBSSH2_CONFIG_H */



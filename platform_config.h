#ifndef PLATFORM_CONFIG_H
#define PLATFORM_CONFIG_H

#if defined(WIN32)
#include "config_win32.h"
#else
#include "config.h"
#endif

#ifndef HAVE_ASSERT_H
#define assert(x) do {} while(0)
#endif

#endif // PLATFORM_CONFIG_H

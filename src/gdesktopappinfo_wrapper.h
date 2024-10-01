#ifndef GDESKTOPAPPINFO_WRAPPER_H
#define GDESKTOPAPPINFO_WRAPPER_H

#ifdef HAVE_X11

#include <gio/gdesktopappinfo.h>

#else // HAVE_X11

#include "gdesktopappinfo_per_gosxappinfo.h"
//#include "gdesktopappinfo_per_gappinfo.h"

#endif // HAVE_X11

#endif


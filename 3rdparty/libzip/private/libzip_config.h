#ifndef HAD_CONFIG_H
#define HAD_CONFIG_H
#ifndef _HAD_ZIPCONF_H
#include "zipconf.h"
#endif

/* Hand-written replacement for libzip's auto-generated config.h.
 * Selects the right set of HAVE_* defines per platform so the same header
 * works for the libretro Makefile build and the cmake build, on
 * Windows (MSVC + MinGW), Linux, and macOS.
 *
 * Renamed from config.h to libzip_config.h to avoid a case-clash with
 * pcsx2's own Config.h on case-insensitive filesystems (Windows / macOS
 * default HFS+/APFS).  zipint.h and compat.h have been patched to
 * #include "libzip_config.h" instead of "config.h". */

/* BEGIN DEFINES */

#define HAVE_LIBZSTD
#define HAVE_STDBOOL_H
#define HAVE_SHARED
#define SIZEOF_SIZE_T 8

/* ---- Windows (MSVC + MinGW) ---- */
#ifdef _WIN32

#define HAVE__CLOSE
#define HAVE__DUP
#define HAVE__FDOPEN
#define HAVE__FILENO
#define HAVE__SETMODE
#define HAVE__SNPRINTF
#define HAVE__STRDUP
#define HAVE__STRICMP
#define HAVE__STRTOI64
#define HAVE__STRTOUI64
#define HAVE__UNLINK
#define HAVE_FILENO
#define HAVE_SETMODE
#define HAVE_SNPRINTF
#define HAVE_STRDUP
#define HAVE_STRICMP
#define HAVE_STRTOLL
#define HAVE_STRTOULL
#define SIZEOF_OFF_T 4

/* MinGW exposes real fseeko/ftello when _FILE_OFFSET_BITS=64 (which the
 * libretro core build sets).  MSVC has neither.  Test '_WIN32 && !_MSC_VER'
 * rather than __MINGW32__ because some MSYS2 mingw64 gcc revisions don't
 * define __MINGW32__.  Without this, compat.h falls into a #define-fseeko
 * branch that corrupts mingw's <stdio.h> prototype further down. */
#ifndef _MSC_VER
#define HAVE_FSEEKO
#define HAVE_FTELLO
#endif

/* ---- POSIX (Linux, macOS, *BSD) ---- */
#else

#define HAVE_FILENO
#define HAVE_FSEEKO
#define HAVE_FTELLO
#define HAVE_LOCALTIME_R
#define HAVE_MKSTEMP
#define HAVE_SETMODE
#define HAVE_SNPRINTF
#define HAVE_STRCASECMP
#define HAVE_STRDUP
#define HAVE_STRTOLL
#define HAVE_STRTOULL
#define HAVE_STRINGS_H
#define HAVE_UNISTD_H
#define HAVE_DIRENT_H
#define SIZEOF_OFF_T 8

/* macOS has clonefile() / arc4random() / commoncrypto, but we don't enable
 * the Apple-specific code paths -- the deflate+zstd configuration is
 * sufficient for pcsx2's needs and keeps the build identical across
 * Linux and macOS.  arc4random would be a faster random source than
 * /dev/urandom on BSD/macOS but zip_random_unix.c falls back gracefully. */

#endif /* _WIN32 */

/* END DEFINES */

#define PACKAGE "libzip"
#define VERSION "1.8.0"

#endif /* HAD_CONFIG_H */

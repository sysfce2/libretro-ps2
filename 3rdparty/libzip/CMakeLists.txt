# Build vendored libzip directly.  The upstream libzip/CMakeLists.txt and
# libzip/lib/CMakeLists.txt files are NOT vendored -- only the .c sources
# under libzip/lib/, plus the pre-generated zip_err_str.c and our hand-
# written portable libzip_config.h under private/.
#
# Source set mirrors the per-platform split used by the libretro Makefile
# (Makefile.common) and the MSVC libzip.vcxproj.  Windows-specific files
# live under #ifdef _WIN32 in their own .c bodies but we still gate them
# at the build-system level so non-Windows targets don't try to link
# zip_crypto_win / zip_random_win32 / zip_source_file_win32* against
# missing system libs (bcrypt, advapi32).
#
# Mirror this list in Makefile.common's libzip block when adding/removing
# files.

set(LIBZIP_COMMON_SOURCES
	libzip/lib/zip_add.c
	libzip/lib/zip_add_dir.c
	libzip/lib/zip_add_entry.c
	libzip/lib/zip_algorithm_deflate.c
	libzip/lib/zip_algorithm_zstd.c
	libzip/lib/zip_buffer.c
	libzip/lib/zip_close.c
	libzip/lib/zip_delete.c
	libzip/lib/zip_dirent.c
	libzip/lib/zip_dir_add.c
	libzip/lib/zip_discard.c
	libzip/lib/zip_entry.c
	libzip/lib/zip_error.c
	libzip/lib/zip_error_clear.c
	libzip/lib/zip_error_get.c
	libzip/lib/zip_error_get_sys_type.c
	libzip/lib/zip_error_strerror.c
	libzip/lib/zip_error_to_str.c
	libzip/lib/zip_extra_field.c
	libzip/lib/zip_extra_field_api.c
	libzip/lib/zip_fclose.c
	libzip/lib/zip_fdopen.c
	libzip/lib/zip_file_add.c
	libzip/lib/zip_file_error_clear.c
	libzip/lib/zip_file_error_get.c
	libzip/lib/zip_file_get_comment.c
	libzip/lib/zip_file_get_external_attributes.c
	libzip/lib/zip_file_get_offset.c
	libzip/lib/zip_file_rename.c
	libzip/lib/zip_file_replace.c
	libzip/lib/zip_file_set_comment.c
	libzip/lib/zip_file_set_encryption.c
	libzip/lib/zip_file_set_external_attributes.c
	libzip/lib/zip_file_set_mtime.c
	libzip/lib/zip_file_strerror.c
	libzip/lib/zip_fopen.c
	libzip/lib/zip_fopen_encrypted.c
	libzip/lib/zip_fopen_index.c
	libzip/lib/zip_fopen_index_encrypted.c
	libzip/lib/zip_fread.c
	libzip/lib/zip_fseek.c
	libzip/lib/zip_ftell.c
	libzip/lib/zip_get_archive_comment.c
	libzip/lib/zip_get_archive_flag.c
	libzip/lib/zip_get_encryption_implementation.c
	libzip/lib/zip_get_file_comment.c
	libzip/lib/zip_get_name.c
	libzip/lib/zip_get_num_entries.c
	libzip/lib/zip_get_num_files.c
	libzip/lib/zip_hash.c
	libzip/lib/zip_io_util.c
	libzip/lib/zip_libzip_version.c
	libzip/lib/zip_memdup.c
	libzip/lib/zip_name_locate.c
	libzip/lib/zip_new.c
	libzip/lib/zip_open.c
	libzip/lib/zip_pkware.c
	libzip/lib/zip_progress.c
	libzip/lib/zip_rename.c
	libzip/lib/zip_replace.c
	libzip/lib/zip_set_archive_comment.c
	libzip/lib/zip_set_archive_flag.c
	libzip/lib/zip_set_default_password.c
	libzip/lib/zip_set_file_comment.c
	libzip/lib/zip_set_file_compression.c
	libzip/lib/zip_set_name.c
	libzip/lib/zip_source_accept_empty.c
	libzip/lib/zip_source_begin_write.c
	libzip/lib/zip_source_begin_write_cloning.c
	libzip/lib/zip_source_buffer.c
	libzip/lib/zip_source_call.c
	libzip/lib/zip_source_close.c
	libzip/lib/zip_source_commit_write.c
	libzip/lib/zip_source_compress.c
	libzip/lib/zip_source_crc.c
	libzip/lib/zip_source_error.c
	libzip/lib/zip_source_file_common.c
	libzip/lib/zip_source_file_stdio.c
	libzip/lib/zip_source_free.c
	libzip/lib/zip_source_function.c
	libzip/lib/zip_source_get_file_attributes.c
	libzip/lib/zip_source_is_deleted.c
	libzip/lib/zip_source_layered.c
	libzip/lib/zip_source_open.c
	libzip/lib/zip_source_pkware_decode.c
	libzip/lib/zip_source_pkware_encode.c
	libzip/lib/zip_source_read.c
	libzip/lib/zip_source_remove.c
	libzip/lib/zip_source_rollback_write.c
	libzip/lib/zip_source_seek.c
	libzip/lib/zip_source_seek_write.c
	libzip/lib/zip_source_stat.c
	libzip/lib/zip_source_supports.c
	libzip/lib/zip_source_tell.c
	libzip/lib/zip_source_tell_write.c
	libzip/lib/zip_source_window.c
	libzip/lib/zip_source_write.c
	libzip/lib/zip_source_zip.c
	libzip/lib/zip_source_zip_new.c
	libzip/lib/zip_stat.c
	libzip/lib/zip_stat_index.c
	libzip/lib/zip_stat_init.c
	libzip/lib/zip_strerror.c
	libzip/lib/zip_string.c
	libzip/lib/zip_unchange.c
	libzip/lib/zip_unchange_all.c
	libzip/lib/zip_unchange_archive.c
	libzip/lib/zip_unchange_data.c
	libzip/lib/zip_utf-8.c
	private/zip_err_str.c
)

if(WIN32)
	# AES-encrypted-archive support (zip_winzip_aes*, zip_source_winzip_aes_*,
	# zip_crypto_win) is intentionally omitted -- pcsx2 never opens encrypted
	# archives (CHD/cso/iso are not WinZip-AES), and bcrypt would be the only
	# extra system lib pulled in.  The legacy pkware "zipcrypto" cipher in
	# zip_pkware.c / zip_source_pkware_*.c does NOT need a crypto backend
	# and stays in LIBZIP_COMMON_SOURCES.
	#
	# zip_random_win32.c uses CryptGenRandom from <wincrypt.h>, which lives
	# in advapi32 -- mingw and MSVC link advapi32 by default, no LIBS edit
	# needed.
	set(LIBZIP_PLATFORM_SOURCES
		libzip/lib/zip_random_win32.c
		libzip/lib/zip_source_file_win32.c
		libzip/lib/zip_source_file_win32_ansi.c
		libzip/lib/zip_source_file_win32_named.c
		libzip/lib/zip_source_file_win32_utf16.c
		libzip/lib/zip_source_file_win32_utf8.c
	)
else()
	set(LIBZIP_PLATFORM_SOURCES
		libzip/lib/zip_random_unix.c
		libzip/lib/zip_source_file_stdio_named.c
	)
endif()

add_library(pcsx2-libzip STATIC
	${LIBZIP_COMMON_SOURCES}
	${LIBZIP_PLATFORM_SOURCES}
)

# zipint.h does `#include "libzip_config.h"` (lowercase) -> 3rdparty/libzip/private/.
# Note: zipint.h also does `#include "compat.h"` -> libzip/lib/.  zip.h is at
# libzip/lib/zip.h.  zipconf.h is at 3rdparty/libzip/zipconf.h (this dir).
# Keep `private/` BEFORE `libzip/lib/` so the lowercase libzip_config.h
# resolves to ours and not to anything that case-folds onto pcsx2's own
# Config.h on Windows / HFS+.
target_include_directories(pcsx2-libzip
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}
		${CMAKE_CURRENT_SOURCE_DIR}/libzip/lib
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/private
)

# ZIP_STATIC keeps zip.h's ZIP_EXTERN as a plain extern instead of
# __declspec(dllimport) when callers include <zip.h>.  Required because we
# build libzip into pcsx2 statically.
target_compile_definitions(pcsx2-libzip
	PUBLIC ZIP_STATIC
	PRIVATE HAVE_CONFIG_H
)

target_link_libraries(pcsx2-libzip
	PRIVATE
		ZLIB::ZLIB
		Zstd::Zstd
)

# Match the alias name the rest of pcsx2 uses (`libzip::zip`).  See
# pcsx2/CMakeLists.txt where this is consumed via target_link_libraries.
alias_library(libzip::zip pcsx2-libzip)

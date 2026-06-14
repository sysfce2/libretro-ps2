/*  PCSX2 - PS2 Emulator for PCs
 *  Copyright (C) 2002-2010  PCSX2 Dev Team
 *
 *  PCSX2 is free software: you can redistribute it and/or modify it under the terms
 *  of the GNU Lesser General Public License as published by the Free Software Found-
 *  ation, either version 3 of the License, or (at your option) any later version.
 *
 *  PCSX2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *  PURPOSE.  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with PCSX2.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#ifdef _WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif

// We require Windows 10+.
#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif
#define _WIN32_WINNT 0x0A00 // Windows 10

#include <windows.h>

/* Runtime-resolved wrappers for the Win10 1803+ placeholder memory APIs
 * (VirtualAlloc2 / MapViewOfFile3 / UnmapViewOfFile2).
 *
 * These are used for fastmem (HostSys.cpp) and the GS local-memory mirror
 * (GS.cpp). Calling them as ordinary imports forces them into the DLL's
 * import table, which means the loader cannot resolve the module on
 * Windows 8 / 8.1 (where the symbols do not exist) and the whole core
 * fails to load before any code runs - even though both subsystems can
 * cope without these APIs (fastmem falls back to slowpath load/stores,
 * GS to a plain contiguous mapping). Resolving them through
 * GetProcAddress instead keeps them out of the import table, so the
 * module loads everywhere and each caller can branch on availability.
 *
 * On Windows 10+ these resolve on first use and behave exactly as the
 * direct calls did; only the (otherwise unreachable on Win10) absence
 * path is new. */
typedef PVOID (WINAPI *PCSX2_VirtualAlloc2_t)(HANDLE, PVOID, SIZE_T,
	ULONG, ULONG, MEM_EXTENDED_PARAMETER*, ULONG);
typedef PVOID (WINAPI *PCSX2_MapViewOfFile3_t)(HANDLE, HANDLE, PVOID,
	ULONG64, SIZE_T, ULONG, ULONG, MEM_EXTENDED_PARAMETER*, ULONG);
typedef BOOL (WINAPI *PCSX2_UnmapViewOfFile2_t)(HANDLE, PVOID, ULONG);

/* Resolved once from kernelbase.dll (where the placeholder APIs live on
 * Win10). Returns true only if all three are present, i.e. the full
 * placeholder workflow is usable. */
static inline bool PCSX2_HasPlaceholderAPIs(
	PCSX2_VirtualAlloc2_t* out_valloc2,
	PCSX2_MapViewOfFile3_t* out_map3,
	PCSX2_UnmapViewOfFile2_t* out_unmap2)
{
	static bool s_resolved = false;
	static PCSX2_VirtualAlloc2_t s_valloc2 = nullptr;
	static PCSX2_MapViewOfFile3_t s_map3 = nullptr;
	static PCSX2_UnmapViewOfFile2_t s_unmap2 = nullptr;

	if (!s_resolved)
	{
		/* kernelbase.dll is always loaded; GetModuleHandle avoids an
		 * extra reference and the symbols forward there on Win10. */
		HMODULE kb = GetModuleHandleW(L"kernelbase.dll");
		if (kb)
		{
			s_valloc2 = reinterpret_cast<PCSX2_VirtualAlloc2_t>(
				reinterpret_cast<void*>(GetProcAddress(kb, "VirtualAlloc2")));
			s_map3 = reinterpret_cast<PCSX2_MapViewOfFile3_t>(
				reinterpret_cast<void*>(GetProcAddress(kb, "MapViewOfFile3")));
			s_unmap2 = reinterpret_cast<PCSX2_UnmapViewOfFile2_t>(
				reinterpret_cast<void*>(GetProcAddress(kb, "UnmapViewOfFile2")));
		}
		s_resolved = true;
	}

	if (out_valloc2)
		*out_valloc2 = s_valloc2;
	if (out_map3)
		*out_map3 = s_map3;
	if (out_unmap2)
		*out_unmap2 = s_unmap2;

	return (s_valloc2 && s_map3 && s_unmap2);
}

#endif

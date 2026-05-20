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

#include "Pcsx2Defs.h"

// ----------------------------------------------------------------------------------------
//  IConsoleWriter -- For printing messages to the libretro log.
// ----------------------------------------------------------------------------------------
// PCSX2 is a threaded environment and multiple threads can write to the log
// asynchronously. Individual calls are written atomically by the underlying
// retro_log_callback, but a multi-line block may be interrupted by logs from
// other threads; compound such blocks into a single string before issuing them.
//
// Each method maps to a fixed retro_log_level:
//   WriteLn  -> RETRO_LOG_INFO
//   Error    -> RETRO_LOG_ERROR
//   Warning  -> RETRO_LOG_WARN
//   Debug    -> RETRO_LOG_DEBUG
//
// All functions return false so logs can be disabled at compile time with the
// "0 && Console.WriteLn(...)" trick.
struct IConsoleWriter
{
	bool WriteLn(const char* fmt, ...) const;
	bool Error(const char* fmt, ...) const;
	bool Warning(const char* fmt, ...) const;
	bool Debug(const char* fmt, ...) const;
};

extern IConsoleWriter Console;

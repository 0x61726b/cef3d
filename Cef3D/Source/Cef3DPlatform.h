//----------------------------------------------------------------------------
// Cef3D
// Copyright (C) 2017 arkenthera
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// https://github.com/arkenthera/cef3d
// Cef3DPlatform.h
// Date: 13.04.2017
//---------------------------------------------------------------------------

#pragma once

// Platform will be defined by CMake.

#define IS_MONOLITHIC 1

#ifndef PLATFORM_WINDOWS
#define PLATFORM_WINDOWS 0
#endif

#ifndef PLATFORM_LINUX
#define PLATFORM_LINUX 0
#endif

#ifndef PLATFORM_MACOS
#define PLATFORM_MACOS 0
#endif

#if PLATFORM_WINDOWS && !IS_MONOLITHIC
#define DLLIMPORT __declspec(dllimport)
#define DLLEXPORT __declspec(dllexport)
#endif

#ifndef DLLIMPORT
#define DLLIMPORT
#endif

#ifndef DLLEXPORT
#define DLLEXPORT
#endif

#if PLATFORM_WINDOWS
	#if _MSC_VER > 1900
		#define HAS_CPP17_FILESYSTEM 1
	#endif
#endif

#ifndef HAS_CPP17_FILESYSTEM
#define HAS_CPP17_FILESYSTEM 1
#endif

#include <string>
#include <list>
#include <set>
#include <vector>

#if PLATFORM_WINDOWS
#include <Windows.h>

#if defined(FORCEINLINE)
#undef FORCEINLINE
#endif
#define FORCEINLINE inline
#endif

#if PLATFORM_WINDOWS
typedef HWND WindowHandle;
#elif PLATFORM_MACOS
typedef NSView* WindowHandle;
#else
typedef void* WindowHandle;
#endif

#if PLATFORM_WINDOWS
typedef HANDLE ProcessHandle;
#elif defined(__linux__) || defined(__APPLE__)
typedef pid_t ProcessHandle;
#endif

#define MEMORY_LEAK_CHECK 1
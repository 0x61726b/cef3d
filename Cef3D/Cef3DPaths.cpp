//----------------------------------------------------------------------------
// Cef3D
// Copyright (C) 2017 arkenthera
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// https://github.com/arkenthera/cef3d
// Cef3DFileSystem.h
// Date: 13.04.2017
//---------------------------------------------------------------------------

#include "Cef3D.h"

namespace Cef3DPrivate
{

}

namespace Cef3D
{
	const std::string& Cef3DPaths::Root()
	{
		static std::string RootDir;

#if PLATFORM_WINDOWS
		TCHAR ModulePath[MAX_PATH];
		GetModuleFileName(GetModuleHandle(NULL), ModulePath, MAX_PATH);

		std::wstring WideModulePath = std::wstring(ModulePath);
		RootDir = std::string(WideModulePath.begin(), WideModulePath.end());
#endif
		// Remove the stuff
		size_t last_slash = RootDir.find_last_of('\\');
		if (last_slash == std::string::npos)
			last_slash = RootDir.find_last_of('/');

		if (last_slash != std::string::npos)
		{
			RootDir = RootDir.substr(0, last_slash);
		}

		return RootDir;
	}
}
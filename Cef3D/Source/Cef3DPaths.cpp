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

#include "Cef3DPCH.h"

namespace Cef3D 
{
	namespace Cef3DPrivate
	{
		bool StringStartsWith(const std::string& A, const std::string& B)
		{
			if (A.compare(0, B.length(), B))
				return true;
			return false;
		}
	}

}

namespace Cef3D
{
	bool Cef3DPaths::IsRelative(const std::string& Path)
	{
		using namespace Cef3D::Cef3DPrivate;

		const bool Rel = StringStartsWith(Path, "\\")
			|| StringStartsWith(Path, "/")
			|| Path.length() >= 2 && isalpha((int)Path[0]) && Path[1] == ':';
		return !Rel;
	}

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
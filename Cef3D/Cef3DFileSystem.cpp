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

namespace Cef3D
{
	bool Cef3DFileSystem::Exists(const char* AbsPath)
	{
#if HAS_CPP17_FILESYSTEM
		return std::experimental::filesystem::exists(AbsPath);
#else
		return false;
#endif
	}


	Cef3DFileSystem& Cef3DFileSystem::Get()
	{
		static Cef3DFileSystem FileSystem;
		return FileSystem;
	}
}

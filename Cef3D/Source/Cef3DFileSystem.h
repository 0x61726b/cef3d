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

#pragma once

#if HAS_CPP17_FILESYSTEM
#include <filesystem>
#else
#endif

namespace Cef3D
{
	class CEF3D_API Cef3DFileSystem
	{
	public:
		bool Exists(const char* AbsPath);
		std::string ReadFile(const std::string& AbsPath);

		static Cef3DFileSystem& Get();
	};
}
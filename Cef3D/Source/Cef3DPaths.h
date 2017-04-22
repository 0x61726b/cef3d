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


namespace Cef3D
{
	class CEF3D_API Cef3DPaths
	{
	public:
		static bool IsRelative(const std::string& Path);

		static const std::string& Root();
		static const std::string& Shaders();
		static const std::string& Log();

		static const std::string Combine(const std::string& A, const std::string& B);
	};
}
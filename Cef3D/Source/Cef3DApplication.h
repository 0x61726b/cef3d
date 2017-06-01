//----------------------------------------------------------------------------
// Cef3D
// Copyright (C) 2017 arkenthera
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// https://github.com/arkenthera/cef3d
// Cef3D.cpp
// Date: 1.6.2017
//---------------------------------------------------------------------------

#pragma once

namespace Cef3D
{
	class Cef3DDelegate
	{
	public:
		Cef3DDelegate();

		virtual void OnReady() { };
		virtual void OnShutdown() { }
	};
}


//----------------------------------------------------------------------------
// Cef3D
// Copyright (C) 2017 arkenthera
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// https://github.com/arkenthera/cef3d
// Cef3D.h
// Date: 21.04.2017
//---------------------------------------------------------------------------

#pragma once


#include "include/cef_base.h"

namespace Cef3D
{
	// Represents a singleton hidden window that acts at temporary parent for
	// popup browsers.
	class CEF3D_API Cef3DTempWindowWin {
	public:
		// Returns the singleton window handle.
		static WindowHandle GetWindowHandle();

	private:
		// A single instance will be created/owned by RootWindowManager.
		friend class RootWindowManager;

		Cef3DTempWindowWin();
		~Cef3DTempWindowWin();

		WindowHandle hwnd_;

		DISALLOW_COPY_AND_ASSIGN(Cef3DTempWindowWin);
	};
}
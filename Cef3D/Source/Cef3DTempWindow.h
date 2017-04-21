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

#if PLATFORM_WINDOWS
#include "Cef3DTempWindow_Win.h"
#endif

namespace Cef3D
{
#if PLATFORM_WINDOWS
	typedef Cef3DTempWindowWin Cef3DTempWindow;
#endif
}
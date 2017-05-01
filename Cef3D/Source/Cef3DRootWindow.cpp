//----------------------------------------------------------------------------
// Cef3D
// Copyright (C) 2017 arkenthera
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// https://github.com/arkenthera/cef3d
// Cef3D.h
// Date: 13.04.2017
//---------------------------------------------------------------------------

#include "Cef3DPCH.h"

namespace Cef3D
{
	scoped_refptr<RootWindow> RootWindow::Create(bool isOsr)
	{
		if (isOsr)
			return new RootWindowWin();
		else
			return new RootWindowViews();
	}

	bool RootWindow::IsSame(scoped_refptr<RootWindow> Wnd)
	{
		if (Wnd->GetBrowser()->GetIdentifier() == Wnd->GetBrowser()->GetIdentifier())
			return true;
		return false;
	}
}
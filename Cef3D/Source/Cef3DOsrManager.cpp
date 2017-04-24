//----------------------------------------------------------------------------
// Cef3D
// Copyright (C) 2017 arkenthera
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// https://github.com/arkenthera/cef3d
// Cef3D.h
// Date: 24.04.2017
//---------------------------------------------------------------------------

#include "Cef3DPCH.h"

namespace Cef3D
{
	Cef3DOsrManager::Cef3DOsrManager()
	{
	}
	int Cef3DOsrManager::CreateBrowser(const Cef3DBrowserDefinition & Def)
	{
		Cef3DOsrBrowserWindow* browserWindow(new Cef3DOsrBrowserWindow);
		browserWindow->Init(Def);
		BrowserList.push_back(browserWindow->GetBrowser());
		return browserWindow->GetBrowser()->GetIdentifier();
	}
}
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

#pragma once

namespace Cef3D
{
	class CEF3D_API Cef3DOsrManager
	{
	public:
		Cef3DOsrManager();

		int CreateBrowser(const Cef3DBrowserDefinition& Def);

	private:
		std::list<CefRefPtr<CefBrowser> > BrowserList;

		DISALLOW_COPY_AND_ASSIGN(Cef3DOsrManager);
	};
}
//----------------------------------------------------------------------------
// Cef3D
// Copyright (C) 2017 arkenthera
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// https://github.com/arkenthera/cef3d
// Cef3DGlobals.h
// Date: 13.04.2017
//---------------------------------------------------------------------------

#pragma once

#include "include/cef_app.h"

namespace Cef3D
{
	class Cef3DBrowser;
	class CEF3D_API Cef3DApplication : 
		public CefApp,
		public CefBrowserProcessHandler
	{
	public:

		Cef3DApplication();

		virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() OVERRIDE { return this; }

		virtual void OnContextInitialized() OVERRIDE;

		int CreateBrowser(const Cef3DBrowserDefinition& Definition, CefRefPtr<Cef3D::Cef3DHandler> CustomHandler);

		Cef3DBrowser* GetCef3DBrowser(CefRefPtr<CefBrowser> Browser);

	public:
		std::map<int,Cef3DBrowser*> BrowserMap;
		std::list<Cef3DBrowser*> TempBrowserList;
	private:
		IMPLEMENT_REFCOUNTING(Cef3DApplication);
		CefRefPtr<Cef3DHandler> Handler;
	};
}
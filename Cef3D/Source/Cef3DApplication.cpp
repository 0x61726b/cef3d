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

#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_helpers.h"

#include "Cef3DPCH.h"

namespace Cef3D
{
	namespace {

		class SimpleWindowDelegate : public ViewsWindow::Delegate {
		public:
			void OnViewsWindowCreated(CefRefPtr<ViewsWindow> window) OVERRIDE;
			void OnViewsWindowDestroyed(CefRefPtr<ViewsWindow> window) OVERRIDE;
			virtual void OnExit() OVERRIDE;
		};

		void SimpleWindowDelegate::OnViewsWindowCreated(CefRefPtr<ViewsWindow> window)
		{

		}

		void SimpleWindowDelegate::OnViewsWindowDestroyed(CefRefPtr<ViewsWindow> window)
		{

		}

		void SimpleWindowDelegate::OnExit()
		{

		}


	}  // namespace
}

namespace Cef3D
{
	Cef3DApplication::Cef3DApplication()
	{

	}

	void Cef3DApplication::OnContextInitialized()
	{
		CEF_REQUIRE_UI_THREAD();
	}

	Cef3DBrowser* Cef3DApplication::GetCef3DBrowser(CefRefPtr<CefBrowser> Browser)
	{
		DCHECK(Browser);

		std::map<int, Cef3DBrowser*>::iterator bit = BrowserMap.find(Browser->GetIdentifier());
		if(bit != BrowserMap.end())
			return bit->second;
		return 0;
	}

	RootWindow* Cef3DApplication::CreateBrowser(const Cef3DBrowserDefinition& Definition)
	{
		CEF_REQUIRE_UI_THREAD();

		std::string testLoadUrl = "http://www.google.com";
		CefBrowserSettings settings = Cef3DPrivate::Cef3DBrowserDefinitionToCef(Definition);

		return GMainContext->GetRootWindowManager()->CreateRootWindow(false, false, CefRect(), testLoadUrl);
	}
}
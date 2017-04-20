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



		// When using the Views framework this object provides the delegate
		// implementation for the CefWindow that hosts the Views-based browser.
		class SimpleWindowDelegate : public CefWindowDelegate {
		public:
			explicit SimpleWindowDelegate(CefRefPtr<CefBrowserView> browser_view, const Cef3DBrowserDefinition& Definition)
				: browser_view_(browser_view),
				BrowserDefinition(Definition)
			{
			}

			void OnWindowCreated(CefRefPtr<CefWindow> window) OVERRIDE {
				// Add the browser view and show the window.

				CefSize browserViewSize(BrowserDefinition.Width, BrowserDefinition.Height);
				browser_view_->SetSize(browserViewSize);

				window->SetSize(browserViewSize);
				window->AddChildView(browser_view_);
				window->Show();

				// Give keyboard focus to the browser view.
				browser_view_->RequestFocus();
			}

			void OnWindowDestroyed(CefRefPtr<CefWindow> window) OVERRIDE {
				browser_view_ = NULL;
			}

			bool CanClose(CefRefPtr<CefWindow> window) OVERRIDE
			{
				// Allow the window to close if the browser says it's OK.
				CefRefPtr<CefBrowser> browser = browser_view_->GetBrowser();
				if (browser)
					return browser->GetHost()->TryCloseBrowser();
				return true;
			}

		private:
			CefRefPtr<CefBrowserView> browser_view_;
			Cef3DBrowserDefinition BrowserDefinition;

			IMPLEMENT_REFCOUNTING(SimpleWindowDelegate);
			DISALLOW_COPY_AND_ASSIGN(SimpleWindowDelegate);
		};

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

		Handler = new Cef3DHandler;
	}

	Cef3DBrowser* Cef3DApplication::GetCef3DBrowser(CefRefPtr<CefBrowser> Browser)
	{
		DCHECK(Browser);

		std::map<int, Cef3DBrowser*>::iterator bit = BrowserMap.find(Browser->GetIdentifier());
		if(bit != BrowserMap.end())
			return bit->second;
		return 0;
	}

	int Cef3DApplication::CreateBrowser(const Cef3DBrowserDefinition& Definition, CefRefPtr<Cef3D::Cef3DHandler> CustomHandler)
	{
		CEF_REQUIRE_UI_THREAD();

#if PLATFORM_WINDOWS || PLATFORM_LINUX
		const bool use_views = true;
#else
		const bool use_views = false;
#endif
		std::string testLoadUrl = "http://www.google.com";
		CefBrowserSettings settings = Cef3DPrivate::Cef3DBrowserDefinitionToCef(Definition);
		if (use_views)
		{
			CefRefPtr<CefBrowserView> browser_view = CefBrowserView::CreateBrowserView(
				CustomHandler, testLoadUrl, settings, NULL, NULL);

			CefRefPtr<CefWindow> cefWindow = CefWindow::CreateTopLevelWindow(new SimpleWindowDelegate(browser_view, Definition));

			return browser_view->GetBrowser()->GetIdentifier();
		}
		else
		{
			CefWindowInfo window_info;

#if defined(OS_WIN)
			window_info.SetAsPopup(NULL, "cefsimple");
#endif

			CefRefPtr<CefBrowser> browser = CefBrowserHost::CreateBrowserSync(window_info, CustomHandler, testLoadUrl, settings,
				NULL);
			return browser->GetIdentifier();
		}

		return -1;
	}
}
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

#include "Cef3D.h"

#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_helpers.h"

namespace {

	// When using the Views framework this object provides the delegate
	// implementation for the CefWindow that hosts the Views-based browser.
	class SimpleWindowDelegate : public CefWindowDelegate {
	public:
		explicit SimpleWindowDelegate(CefRefPtr<CefBrowserView> browser_view)
			: browser_view_(browser_view) {
		}

		void OnWindowCreated(CefRefPtr<CefWindow> window) OVERRIDE {
			// Add the browser view and show the window.
			window->AddChildView(browser_view_);
			window->Show();

			// Give keyboard focus to the browser view.
			browser_view_->RequestFocus();
		}

		void OnWindowDestroyed(CefRefPtr<CefWindow> window) OVERRIDE {
			browser_view_ = NULL;
		}

		bool CanClose(CefRefPtr<CefWindow> window) OVERRIDE {
			// Allow the window to close if the browser says it's OK.
			CefRefPtr<CefBrowser> browser = browser_view_->GetBrowser();
			if (browser)
				return browser->GetHost()->TryCloseBrowser();
			return true;
		}

	private:
		CefRefPtr<CefBrowserView> browser_view_;

		IMPLEMENT_REFCOUNTING(SimpleWindowDelegate);
		DISALLOW_COPY_AND_ASSIGN(SimpleWindowDelegate);
	};

}  // namespace

namespace Cef3D
{
	Cef3DApplication::Cef3DApplication()
	{

	}

	void Cef3DApplication::OnContextInitialized()
	{
		CEF_REQUIRE_UI_THREAD();

		CefRefPtr<CefCommandLine> command_line =
			CefCommandLine::GetGlobalCommandLine();

#if PLATFORM_WINDOWS || PLATFORM_LINUX
		// Create the browser using the Views framework if "--use-views" is specified
		// via the command-line. Otherwise, create the browser using the native
		// platform framework. The Views framework is currently only supported on
		// Windows and Linux.
		const bool use_views = true;
#else
		const bool use_views = false;
#endif

		// SimpleHandler implements browser-level callbacks.
		CefRefPtr<Cef3DHandler> handler(new Cef3DHandler);

		// Specify CEF browser settings here.
		CefBrowserSettings browser_settings;

		std::string url;

		// Check if a "--url=" value was provided via the command-line. If so, use
		// that instead of the default URL.
		url = command_line->GetSwitchValue("url");
		if (url.empty())
			url = "http://www.google.com";

		if (use_views) {
			// Create the BrowserView.
			CefRefPtr<CefBrowserView> browser_view = CefBrowserView::CreateBrowserView(
				handler, url, browser_settings, NULL, NULL);

			// Create the Window. It will show itself after creation.
			CefWindow::CreateTopLevelWindow(new SimpleWindowDelegate(browser_view));
		}
		else {
			// Information used when creating the native window.
			CefWindowInfo window_info;

#if defined(OS_WIN)
			// On Windows we need to specify certain flags that will be passed to
			// CreateWindowEx().
			window_info.SetAsPopup(NULL, "cefsimple");
#endif

			// Create the first browser window.
			CefBrowserHost::CreateBrowser(window_info, handler, url, browser_settings,
				NULL);
		}
	}
}
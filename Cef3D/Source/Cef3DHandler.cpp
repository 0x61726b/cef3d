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

#include "Cef3DPCH.h"

#include "include/base/cef_bind.h"
#include "include/cef_app.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"

namespace Cef3D
{
	static Cef3DHandler* Instance = 0;

	Cef3DHandler::Cef3DHandler()
	{
		Instance = this;
		DCHECK(Instance);
	}

	Cef3DHandler::~Cef3DHandler()
	{
		Instance = 0;
	}

	void Cef3DHandler::OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title)
	{
		CEF_REQUIRE_UI_THREAD();
	}


	void Cef3DHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser)
	{
		CEF_REQUIRE_UI_THREAD();

		browser_list_.push_back(browser);
	}

	bool Cef3DHandler::DoClose(CefRefPtr<CefBrowser> browser)
	{
		CEF_REQUIRE_UI_THREAD();

		return false;
	}

	void Cef3DHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser)
	{
		CEF_REQUIRE_UI_THREAD();

		BrowserList::iterator bit = browser_list_.begin();
		for (; bit != browser_list_.end(); ++bit)
		{
			if ((*bit)->IsSame(browser)) 
			{
				browser_list_.erase(bit);
				break;
			}
		}

		if (browser_list_.empty()) 
		{
			// All browser windows have closed. Quit the application message loop.
			CefQuitMessageLoop();
		}
	}

	void Cef3DHandler::OnLoadError(CefRefPtr<CefBrowser> browser,CefRefPtr<CefFrame> frame,ErrorCode errorCode,const CefString& errorText,const CefString& failedUrl)
	{
		CEF_REQUIRE_UI_THREAD();

		if (errorCode == ERR_ABORTED)
			return;

		std::stringstream ss;
		ss << "<html><body bgcolor=\"white\">"
			"<h2>Failed to load URL " << std::string(failedUrl) <<
			" with error " << std::string(errorText) << " (" << errorCode <<
			").</h2></body></html>";
		frame->LoadString(ss.str(), failedUrl);
	}

	void Cef3DHandler::CloseAllBrowsers(bool force_close) 
	{
		if (!CefCurrentlyOn(TID_UI))
		{
			// Execute on the UI thread.
			CefPostTask(TID_UI,
				base::Bind(&Cef3DHandler::CloseAllBrowsers, this, force_close));
			return;
		}

		if (browser_list_.empty())
			return;

		BrowserList::const_iterator it = browser_list_.begin();
		for (; it != browser_list_.end(); ++it)
			(*it)->GetHost()->CloseBrowser(force_close);
	}


	Cef3DHandler* Cef3DHandler::Get()
	{
		return Instance;
	}
}
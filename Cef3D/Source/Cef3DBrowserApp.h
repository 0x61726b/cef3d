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
	class CEF3D_API Cef3DBrowserApp : 
		public CefApp,
		public CefBrowserProcessHandler
	{
	public:
		static CefProcessType GetProcessType(CefRefPtr<CefCommandLine> command_line);

		class BrowserDelegate : public virtual CefBaseRefCounted
		{
		public:
			virtual void OnBeforeCommandLineProcessing(
				CefRefPtr<Cef3DBrowserApp> app,
				CefRefPtr<CefCommandLine> command_line) {}

			virtual void OnContextInitialized(CefRefPtr<Cef3DBrowserApp> app) {}

			virtual void OnBeforeChildProcessLaunch(
				CefRefPtr<Cef3DBrowserApp> app,
				CefRefPtr<CefCommandLine> command_line) {}

			virtual void OnRenderProcessThreadCreated(
				CefRefPtr<Cef3DBrowserApp> app,
				CefRefPtr<CefListValue> extra_info) {}
		};

		typedef std::set<CefRefPtr<BrowserDelegate> > DelegateSet;

	private:
		void RegisterDelegate(CefRefPtr<BrowserDelegate> del);

		// CefApp methods.
		void OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line) OVERRIDE;

		CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() OVERRIDE { return this; }

		// CefBrowserProcessHandler methods.
		void OnContextInitialized() OVERRIDE;
		void OnBeforeChildProcessLaunch(CefRefPtr<CefCommandLine> command_line) OVERRIDE;
		void OnRenderProcessThreadCreated(CefRefPtr<CefListValue> extra_info) OVERRIDE;
		void OnScheduleMessagePumpWork(int64 delay) OVERRIDE;

		// Set of supported Delegates.
		DelegateSet DelegateList;
		std::vector<CefString> CookieableSchemes;
	public:
		Cef3DBrowserApp();

	private:
		IMPLEMENT_REFCOUNTING(Cef3DBrowserApp);
	};
}
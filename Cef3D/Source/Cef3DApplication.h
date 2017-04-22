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
	class CEF3D_API Cef3DApplication : 
		public CefApp,
		public CefBrowserProcessHandler
	{
	public:
		static CefProcessType GetProcessType(CefRefPtr<CefCommandLine> command_line);

		// Interface for browser delegates. All Delegates must be returned via
		// CreateDelegates. Do not perform work in the Delegate
		// constructor. See CefBrowserProcessHandler for documentation.
		class Delegate : public virtual CefBaseRefCounted {
		public:
			virtual void OnBeforeCommandLineProcessing(
				CefRefPtr<Cef3DApplication> app,
				CefRefPtr<CefCommandLine> command_line) {}

			virtual void OnContextInitialized(CefRefPtr<Cef3DApplication> app) {}

			virtual void OnBeforeChildProcessLaunch(
				CefRefPtr<Cef3DApplication> app,
				CefRefPtr<CefCommandLine> command_line) {}

			virtual void OnRenderProcessThreadCreated(
				CefRefPtr<Cef3DApplication> app,
				CefRefPtr<CefListValue> extra_info) {}
		};

		typedef std::set<CefRefPtr<Delegate> > DelegateSet;

	private:
		// Creates all of the Delegate objects. Implemented by cefclient in
		// client_app_delegates_browser.cc
		void RegisterDelegate(CefRefPtr<Delegate> Del);


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
		Cef3DApplication();

	private:
		IMPLEMENT_REFCOUNTING(Cef3DApplication);
	};
}
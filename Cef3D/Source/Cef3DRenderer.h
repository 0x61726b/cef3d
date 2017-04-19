//----------------------------------------------------------------------------
// Cef3D
// Copyright (C) 2017 arkenthera
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// https://github.com/arkenthera/cef3d
// Cef3DRenderer.h
// Date: 18.04.2017
//---------------------------------------------------------------------------

#pragma once

#include "include/cef_app.h"

namespace Cef3D
{
	class CEF3D_API Cef3DRenderer : 
		public CefApp,
		public CefRenderProcessHandler
	{
	public:
		Cef3DRenderer();

		// CefRenderProcessHandler methods.
		void OnRenderThreadCreated(CefRefPtr<CefListValue> extra_info) OVERRIDE;
		void OnWebKitInitialized() OVERRIDE;
		void OnBrowserCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
		void OnBrowserDestroyed(CefRefPtr<CefBrowser> browser) OVERRIDE;
		CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE;
		bool OnBeforeNavigation(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, NavigationType navigation_type, bool is_redirect) OVERRIDE;
		void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) OVERRIDE;
		void OnContextReleased(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) OVERRIDE;
		void OnUncaughtException(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context, CefRefPtr<CefV8Exception> exception,CefRefPtr<CefV8StackTrace> stackTrace) OVERRIDE;
		void OnFocusedNodeChanged(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefDOMNode> node) OVERRIDE;
		bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message) OVERRIDE;

	private:
		IMPLEMENT_REFCOUNTING(Cef3DRenderer);
		DISALLOW_COPY_AND_ASSIGN(Cef3DRenderer);
	};
}
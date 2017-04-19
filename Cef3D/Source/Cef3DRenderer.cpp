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

#include "Cef3DPCH.h"

namespace Cef3D
{
	Cef3DRenderer::Cef3DRenderer()
	{

	}


	void Cef3DRenderer::OnRenderThreadCreated(CefRefPtr<CefListValue> extra_info) 
	{
		
	}

	void Cef3DRenderer::OnWebKitInitialized() 
	{
		
	}

	void Cef3DRenderer::OnBrowserCreated(CefRefPtr<CefBrowser> browser) 
	{
		
	}

	void Cef3DRenderer::OnBrowserDestroyed(CefRefPtr<CefBrowser> browser) 
	{
		
	}

	CefRefPtr<CefLoadHandler> Cef3DRenderer::GetLoadHandler() 
	{
		CefRefPtr<CefLoadHandler> load_handler;
		return load_handler;
	}

	bool Cef3DRenderer::OnBeforeNavigation(CefRefPtr<CefBrowser> browser,CefRefPtr<CefFrame> frame,CefRefPtr<CefRequest> request,NavigationType navigation_type,bool is_redirect) 
	{
		return true;
	}

	void Cef3DRenderer::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
	{

	}

	void Cef3DRenderer::OnContextReleased(CefRefPtr<CefBrowser> browser,CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) 
	{

	}

	void Cef3DRenderer::OnUncaughtException(CefRefPtr<CefBrowser> browser,CefRefPtr<CefFrame> frame,CefRefPtr<CefV8Context> context,CefRefPtr<CefV8Exception> exception,CefRefPtr<CefV8StackTrace> stackTrace) 
	{

	}

	void Cef3DRenderer::OnFocusedNodeChanged(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefDOMNode> node) 
	{

	}

	bool Cef3DRenderer::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message) 
	{
		DCHECK_EQ(source_process, PID_BROWSER);

		return true;
	}
}
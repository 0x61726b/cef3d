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

#include <iostream>

namespace Cef3D
{
	Cef3DRenderer::Cef3DRenderer()
	{

	}

	void Cef3DRenderer::RegisterDelegate(CefRefPtr<Cef3DRendererDelegate> Del)
	{
		Delegates.insert(Del);
	}


	void Cef3DRenderer::OnRenderThreadCreated(CefRefPtr<CefListValue> extra_info) 
	{
		DelegateSet::iterator it = Delegates.begin();
		for (; it != Delegates.end(); ++it)
			(*it)->OnRenderThreadCreated(this, extra_info);
	}

	void Cef3DRenderer::OnWebKitInitialized() 
	{
		DelegateSet::iterator it = Delegates.begin();
		for (; it != Delegates.end(); ++it)
			(*it)->OnWebKitInitialized(this);
	}

	void Cef3DRenderer::OnBrowserCreated(CefRefPtr<CefBrowser> browser) 
	{
		DelegateSet::iterator it = Delegates.begin();
		for (; it != Delegates.end(); ++it)
			(*it)->OnBrowserCreated(this, browser);
	}

	void Cef3DRenderer::OnBrowserDestroyed(CefRefPtr<CefBrowser> browser) 
	{
		DelegateSet::iterator it = Delegates.begin();
		for (; it != Delegates.end(); ++it)
			(*it)->OnBrowserDestroyed(this, browser);
	}

	CefRefPtr<CefLoadHandler> Cef3DRenderer::GetLoadHandler() 
	{
		CefRefPtr<CefLoadHandler> load_handler;
		DelegateSet::iterator it = Delegates.begin();
		for (; it != Delegates.end() && !load_handler.get(); ++it)
			load_handler = (*it)->GetLoadHandler(this);

		return load_handler;
	}

	bool Cef3DRenderer::OnBeforeNavigation(CefRefPtr<CefBrowser> browser,CefRefPtr<CefFrame> frame,CefRefPtr<CefRequest> request,NavigationType navigation_type,bool is_redirect) 
	{
		DelegateSet::iterator it = Delegates.begin();
		for (; it != Delegates.end(); ++it) {
			if ((*it)->OnBeforeNavigation(this, browser, frame, request,
				navigation_type, is_redirect)) {
				return true;
			}
		}

		return false;
	}

	void Cef3DRenderer::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
	{
		DelegateSet::iterator it = Delegates.begin();
		for (; it != Delegates.end(); ++it)
			(*it)->OnContextCreated(this, browser, frame, context);

		CefRefPtr<CefCommandLine> cmd = CefCommandLine::GetGlobalCommandLine();

		CefRefPtr<CefV8Value> object = context->GetGlobal();
		CefRefPtr<CefV8Value> str = CefV8Value::CreateString(cmd->GetSwitchValue("lastfm_album_name"));
		object->SetValue("cppSongName", str, V8_PROPERTY_ATTRIBUTE_NONE);
	}

	void Cef3DRenderer::OnContextReleased(CefRefPtr<CefBrowser> browser,CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) 
	{
		DelegateSet::iterator it = Delegates.begin();
		for (; it != Delegates.end(); ++it)
			(*it)->OnContextReleased(this, browser, frame, context);
	}

	void Cef3DRenderer::OnUncaughtException(CefRefPtr<CefBrowser> browser,CefRefPtr<CefFrame> frame,CefRefPtr<CefV8Context> context,CefRefPtr<CefV8Exception> exception,CefRefPtr<CefV8StackTrace> stackTrace) 
	{
		DelegateSet::iterator it = Delegates.begin();
		for (; it != Delegates.end(); ++it) {
			(*it)->OnUncaughtException(this, browser, frame, context, exception,
				stackTrace);
		}
	}

	void Cef3DRenderer::OnFocusedNodeChanged(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefDOMNode> node) 
	{
		DelegateSet::iterator it = Delegates.begin();
		for (; it != Delegates.end(); ++it)
			(*it)->OnFocusedNodeChanged(this, browser, frame, node);
	}

	bool Cef3DRenderer::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message) 
	{
		DCHECK_EQ(source_process, PID_BROWSER);

		bool handled = false;

		DelegateSet::iterator it = Delegates.begin();
		for (; it != Delegates.end() && !handled; ++it) {
			handled = (*it)->OnProcessMessageReceived(this, browser, source_process,
				message);
		}

		return handled;
	}
}
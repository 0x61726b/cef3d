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
	class Cef3DRenderer;
	class CEF3D_API Cef3DRendererDelegate : public virtual CefBaseRefCounted
	{
	public:
		virtual void OnRenderThreadCreated(CefRefPtr<Cef3DRenderer> app,
			CefRefPtr<CefListValue> extra_info) {}

		virtual void OnWebKitInitialized(CefRefPtr<Cef3DRenderer> app) {}

		virtual void OnBrowserCreated(CefRefPtr<Cef3DRenderer> app,
			CefRefPtr<CefBrowser> browser) {}

		virtual void OnBrowserDestroyed(CefRefPtr<Cef3DRenderer> app,
			CefRefPtr<CefBrowser> browser) {}

		virtual CefRefPtr<CefLoadHandler> GetLoadHandler(
			CefRefPtr<Cef3DRenderer> app) {
			return NULL;
		}

		virtual bool OnBeforeNavigation(CefRefPtr<Cef3DRenderer> app,
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefRequest> request,
			cef_navigation_type_t navigation_type,
			bool is_redirect) {
			return false;
		}

		virtual void OnContextCreated(CefRefPtr<Cef3DRenderer> app,
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefV8Context> context) {}

		virtual void OnContextReleased(CefRefPtr<Cef3DRenderer> app,
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefV8Context> context) {}

		virtual void OnUncaughtException(CefRefPtr<Cef3DRenderer> app,
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefV8Context> context,
			CefRefPtr<CefV8Exception> exception,
			CefRefPtr<CefV8StackTrace> stackTrace) {}

		virtual void OnFocusedNodeChanged(CefRefPtr<Cef3DRenderer> app,
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefDOMNode> node) {}

		// Called when a process message is received. Return true if the message was
		// handled and should not be passed on to other handlers. Delegates
		// should check for unique message names to avoid interfering with each
		// other.
		virtual bool OnProcessMessageReceived(
			CefRefPtr<Cef3DRenderer> app,
			CefRefPtr<CefBrowser> browser,
			CefProcessId source_process,
			CefRefPtr<CefProcessMessage> message) {
			return false;
		}
	};

	class CEF3D_API Cef3DRenderer :
		public CefApp,
		public CefRenderProcessHandler
	{
	public:
		Cef3DRenderer();

		typedef std::set<CefRefPtr<Cef3DRendererDelegate> > DelegateSet;

		void RegisterDelegate(CefRefPtr<Cef3DRendererDelegate> Del);

		virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() OVERRIDE { return this; }

		// CefRenderProcessHandler methods.
		void OnRenderThreadCreated(CefRefPtr<CefListValue> extra_info) OVERRIDE;
		void OnWebKitInitialized() OVERRIDE;
		void OnBrowserCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
		void OnBrowserDestroyed(CefRefPtr<CefBrowser> browser) OVERRIDE;
		CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE;
		bool OnBeforeNavigation(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, NavigationType navigation_type, bool is_redirect) OVERRIDE;
		void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) OVERRIDE;
		void OnContextReleased(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) OVERRIDE;
		void OnUncaughtException(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context, CefRefPtr<CefV8Exception> exception, CefRefPtr<CefV8StackTrace> stackTrace) OVERRIDE;
		void OnFocusedNodeChanged(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefDOMNode> node) OVERRIDE;
		bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message) OVERRIDE;

		bool GlobalJsObjectExists(CefRefPtr<CefBrowser> browser, const Cef3DJsValue& value);
		void AddGlobalJsObject(CefRefPtr<CefBrowser> browser, const Cef3DJsValue& value);
		void UpdateGlobalJsObject(CefRefPtr<CefBrowser> browser, const Cef3DJsValue& value);
		void RemoveGlobalJsObject(CefRefPtr<CefBrowser> browser, const Cef3DJsValue& value);

		void ApplyGlobalJsObjects();
	private:
		DelegateSet Delegates;

		typedef std::map< CefRefPtr<CefBrowser>, std::vector< Cef3DJsValue > > GlobalJsObjectMap;
		GlobalJsObjectMap GlobalJsObjects;

		IMPLEMENT_REFCOUNTING(Cef3DRenderer);
		DISALLOW_COPY_AND_ASSIGN(Cef3DRenderer);
	};
}
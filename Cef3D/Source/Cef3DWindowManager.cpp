//----------------------------------------------------------------------------
// Cef3D
// Copyright (C) 2017 arkenthera
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// https://github.com/arkenthera/cef3d
// Cef3D.h
// Date: 21.04.2017
//---------------------------------------------------------------------------

#include "Cef3DPCH.h"

namespace Cef3D
{

	namespace {

		class ClientRequestContextHandler : public CefRequestContextHandler {
		public:
			ClientRequestContextHandler() {}

			bool OnBeforePluginLoad(const CefString& mime_type,
				const CefString& plugin_url,
				bool is_main_frame,
				const CefString& top_origin_url,
				CefRefPtr<CefWebPluginInfo> plugin_info,
				PluginPolicy* plugin_policy) OVERRIDE {
				// Always allow the PDF plugin to load.
				if (*plugin_policy != PLUGIN_POLICY_ALLOW &&
					mime_type == "application/pdf") {
					*plugin_policy = PLUGIN_POLICY_ALLOW;
					return true;
				}

				return false;
			}

		private:
			IMPLEMENT_REFCOUNTING(ClientRequestContextHandler);
		};
	}


	RootWindowManager::RootWindowManager()
	{
		CefRefPtr<CefCommandLine> command_line =
			CefCommandLine::GetGlobalCommandLine();
		DCHECK(command_line.get());
		RequestContextPerBrowser = command_line->HasSwitch("request-context-per-browser");
		RequestContextSharedCache = command_line->HasSwitch("request-context-shared-cache");
	}

	RootWindowManager::~RootWindowManager()
	{
		// All root windows should already have been destroyed.
		DCHECK(WindowList.empty());
	}

	scoped_refptr<RootWindow> RootWindowManager::CreateRootWindow(const Cef3DBrowserDefinition& def)
	{
		CefBrowserSettings settings;
		GMainContext->PopulateBrowserSettings(&settings);

		scoped_refptr<RootWindow> root_window = RootWindow::Create(true);
		root_window->Init(this, def);

		// Store a reference to the root window on the main thread.
		OnRootWindowCreated(root_window);

		return root_window;
	}

	scoped_refptr<RootWindow> RootWindowManager::CreateRootWindowAsPopup(bool with_osr, const CefPopupFeatures& popupFeatures, CefWindowInfo& windowInfo,
		CefRefPtr<CefClient>& client,
		CefBrowserSettings& settings)
	{
		GMainContext->PopulateBrowserSettings(&settings);

		scoped_refptr<RootWindow> root_window = RootWindow::Create(true);
		root_window->InitAsPopup(this, with_osr,
			popupFeatures, windowInfo, client, settings);

		// Store a reference to the root window on the main thread.
		OnRootWindowCreated(root_window);

		return root_window;
	}

	scoped_refptr<RootWindow> RootWindowManager::GetWindowForBrowser(int browser_id)
	{
		REQUIRE_MAIN_THREAD();

		RootWindowSet::const_iterator it = WindowList.begin();
		for (; it != WindowList.end(); ++it)
		{
			CefRefPtr<CefBrowser> browser = (*it)->GetBrowser();
			if (browser.get() && browser->GetIdentifier() == browser_id)
				return *it;
		}
		return NULL;
	}

	void RootWindowManager::CloseAllWindows(bool force)
	{
		if (!CURRENTLY_ON_MAIN_THREAD()) {
			// Execute this method on the main thread.
			MAIN_POST_CLOSURE(
				base::Bind(&RootWindowManager::CloseAllWindows, base::Unretained(this),
					force));
			return;
		}

		if (WindowList.empty())
			return;

		RootWindowSet::const_iterator it = WindowList.begin();
		for (; it != WindowList.end(); ++it)
			(*it)->Close(force);
	}

	void RootWindowManager::OnRootWindowCreated(
		scoped_refptr<RootWindow> root_window)
	{
		if (!CURRENTLY_ON_MAIN_THREAD())
		{
			// Execute this method on the main thread.
			MAIN_POST_CLOSURE(
				base::Bind(&RootWindowManager::OnRootWindowCreated,
					base::Unretained(this), root_window));
			return;
		}

		WindowList.insert(root_window);
	}

	CefRefPtr<CefRequestContext> RootWindowManager::GetRequestContext(
		RootWindow* root_window) {
		REQUIRE_MAIN_THREAD();

		if (RequestContextPerBrowser)
		{
			// Create a new request context for each browser.
			CefRequestContextSettings settings;

			CefRefPtr<CefCommandLine> command_line =
				CefCommandLine::GetGlobalCommandLine();
			if (command_line->HasSwitch("cache-path"))
			{
				if (RequestContextSharedCache)
				{
					// Give each browser the same cache path. The resulting context objects
					// will share the same storage internally.
					CefString(&settings.cache_path) =
						command_line->GetSwitchValue("cache-path");
				}
				else
				{
					// Give each browser a unique cache path. This will create completely
					// isolated context objects.
					std::stringstream ss;
					ss << command_line->GetSwitchValue("cache-path").ToString() <<
						time(NULL);
					CefString(&settings.cache_path) = ss.str();
				}
			}

			return CefRequestContext::CreateContext(settings,new ClientRequestContextHandler);
		}

		// All browsers will share the global request context.
		if (!SharedRequestContext.get())
		{
			SharedRequestContext = CefRequestContext::CreateContext(CefRequestContext::GetGlobalContext(), new ClientRequestContextHandler);
		}
		return SharedRequestContext;
	}

	void RootWindowManager::OnExit(RootWindow* root_window)
	{
		REQUIRE_MAIN_THREAD();

		CloseAllWindows(false);
	}

	void RootWindowManager::OnRootWindowDestroyed(RootWindow* root_window) {
		REQUIRE_MAIN_THREAD();

		RootWindowSet::iterator it = WindowList.find(root_window);
		DCHECK(it != WindowList.end());
		if (it != WindowList.end())
			WindowList.erase(it);

		if (WindowList.empty())
		{
			if(GMainContext->IsUsingCefLoop())
				CefQuitMessageLoop();
		}
	}
}
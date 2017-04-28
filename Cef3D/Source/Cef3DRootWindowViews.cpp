//----------------------------------------------------------------------------
// Cef3D
// Copyright (C) 2017 arkenthera
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// https://github.com/arkenthera/cef3d
// Cef3D.h
// Date: 13.04.2017
//---------------------------------------------------------------------------

#include "Cef3DPCH.h"
#include "include/base/cef_bind.h"
#include "include/base/cef_build.h"
#include "include/wrapper/cef_helpers.h"
#include "include/cef_app.h"

namespace Cef3D
{

	RootWindowViews::RootWindowViews()
		: Delegate(NULL),
		IsPopup(false),
		IsInitialized(false),
		IsWindowDestroyed(false),
		IsBrowserDestroyed(false)
	{
	}

	RootWindowViews::~RootWindowViews()
	{
		REQUIRE_MAIN_THREAD();
	}

	void RootWindowViews::Init(RootWindow::Delegate* delegate, bool with_osr, const CefRect& bounds, const CefBrowserSettings& settings, const std::string& url)
	{
		DCHECK(delegate);
		DCHECK(!with_osr);  // Windowless rendering is not supported.
		DCHECK(!IsInitialized);

		Delegate = delegate;
		InitialBounds = bounds;
		CreateClientHandler(url);
		IsInitialized = true;

		// Continue initialization on the main thread.
		InitOnMainThread(settings, url);
	}

	void RootWindowViews::InitAsPopup(RootWindow::Delegate* delegate, bool with_osr, const CefPopupFeatures& popupFeatures, CefWindowInfo& windowInfo,
		CefRefPtr<CefClient>& client,
		CefBrowserSettings& settings)
	{
		DCHECK(delegate);
		DCHECK(!with_osr);  // Windowless rendering is not supported.
		DCHECK(!IsInitialized);

		Delegate = delegate;
		IsPopup = true;

		if (popupFeatures.xSet)
			InitialBounds.x = popupFeatures.x;
		if (popupFeatures.ySet)
			InitialBounds.y = popupFeatures.y;
		if (popupFeatures.widthSet)
			InitialBounds.width = popupFeatures.width;
		if (popupFeatures.heightSet)
			InitialBounds.height = popupFeatures.height;

		CreateClientHandler(std::string());
		IsInitialized = true;

		// The Window will be created in ViewsWindow::OnPopupBrowserViewCreated().
		client = ClientHandler;
	}

	void RootWindowViews::Show(ShowMode mode)
	{
		if (!CefCurrentlyOn(TID_UI))
		{
			// Execute this method on the UI thread.
			CefPostTask(TID_UI, base::Bind(&RootWindowViews::Show, this, mode));
			return;
		}

		if (!Window)
			return;

		Window->Show();

		switch (mode)
		{
		case ShowMinimized:
			Window->Minimize();
			break;
		case ShowMaximized:
			Window->Maximize();
			break;
		default:
			break;
		}
	}

	void RootWindowViews::Hide()
	{
		if (!CefCurrentlyOn(TID_UI))
		{
			// Execute this method on the UI thread.
			CefPostTask(TID_UI, base::Bind(&RootWindowViews::Hide, this));
			return;
		}

		if (Window)
			Window->Hide();
	}

	void RootWindowViews::SetBounds(int x, int y, size_t width, size_t height)
	{
		if (!CefCurrentlyOn(TID_UI))
		{
			// Execute this method on the UI thread.
			CefPostTask(TID_UI,
				base::Bind(&RootWindowViews::SetBounds, this, x, y, width, height));
			return;
		}

		if (Window)
		{
			Window->SetBounds(CefRect(x, y, static_cast<int>(width), static_cast<int>(height)));
		}
	}

	void RootWindowViews::Close(bool force)
	{
		if (!CefCurrentlyOn(TID_UI))
		{
			// Execute this method on the UI thread.
			CefPostTask(TID_UI,
				base::Bind(&RootWindowViews::Close, this, force));
			return;
		}

		if (Window)
			Window->Close(force);
	}

	void RootWindowViews::SetDeviceScaleFactor(float device_scale_factor)
	{
		REQUIRE_MAIN_THREAD();
		// Windowless rendering is not supported.
		NOTREACHED();
	}

	float RootWindowViews::GetDeviceScaleFactor() const
	{
		REQUIRE_MAIN_THREAD();
		// Windowless rendering is not supported.
		NOTREACHED();
		return 0.0;
	}

	CefRefPtr<CefBrowser> RootWindowViews::GetBrowser() const
	{
		REQUIRE_MAIN_THREAD();
		return Browser;
	}

	WindowHandle RootWindowViews::GetWindowHandle() const
	{
		REQUIRE_MAIN_THREAD();
#if PLATFORM_LINUX
		// ClientWindowHandle is a GtkWidget* on Linux and we don't have one of those.
		return NULL;
#else
		if (Browser)
			return Browser->GetHost()->GetWindowHandle();
		return kNullWindowHandle;
#endif
	}

	CefRect RootWindowViews::GetWindowBounds()
	{
		CEF_REQUIRE_UI_THREAD();
		return InitialBounds;
	}

	void RootWindowViews::OnViewsWindowCreated(CefRefPtr<ViewsWindow> window)
	{
		CEF_REQUIRE_UI_THREAD();
		DCHECK(!Window);
		Window = window;
	}

	void RootWindowViews::OnViewsWindowDestroyed(CefRefPtr<ViewsWindow> window)
	{
		CEF_REQUIRE_UI_THREAD();
		Window = NULL;

		// Continue on the main thread.
		MAIN_POST_CLOSURE(
			base::Bind(&RootWindowViews::NotifyViewsWindowDestroyed, this));
	}

	ViewsWindow::Delegate* RootWindowViews::GetDelegateForPopup(CefRefPtr<CefClient> client)
	{
		CEF_REQUIRE_UI_THREAD();
		// |handler| was created in RootWindowViews::InitAsPopup().
		Cef3DHandler* handler = static_cast<Cef3DHandler*>(client.get());
		RootWindowViews* root_window =
			static_cast<RootWindowViews*>(handler->delegate());
		return root_window;
	}

	void RootWindowViews::OnExit()
	{
		if (!CURRENTLY_ON_MAIN_THREAD())
		{
			// Execute this method on the main thread.
			MAIN_POST_CLOSURE(base::Bind(&RootWindowViews::OnExit, this));
			return;
		}

		Delegate->OnExit(this);
	}

	void RootWindowViews::OnBrowserCreated(CefRefPtr<CefBrowser> browser)
	{
		REQUIRE_MAIN_THREAD();
		DCHECK(!Browser);
		Browser = browser;

		
		Cef3DDelegates::OnBrowserCreated.Broadcast(0);
	}

	void RootWindowViews::OnBrowserClosing(CefRefPtr<CefBrowser> browser)
	{
		REQUIRE_MAIN_THREAD();
		// Nothing to do here.
	}

	void RootWindowViews::OnBrowserClosed(CefRefPtr<CefBrowser> browser)
	{
		REQUIRE_MAIN_THREAD();
		if (Browser)
		{
			DCHECK_EQ(browser->GetIdentifier(), Browser->GetIdentifier());
			Browser = NULL;
		}

		ClientHandler->DetachDelegate();
		ClientHandler = NULL;

		IsBrowserDestroyed = true;
		NotifyDestroyedIfDone();
	}

	void RootWindowViews::OnSetAddress(const std::string& url)
	{
		if (!CefCurrentlyOn(TID_UI))
		{
			// Execute this method on the UI thread.
			CefPostTask(TID_UI,
				base::Bind(&RootWindowViews::OnSetAddress, this, url));
			return;
		}

		if (Window)
			Window->SetAddress(url);
	}

	void RootWindowViews::OnSetTitle(const std::string& title)
	{
		if (!CefCurrentlyOn(TID_UI))
		{
			// Execute this method on the UI thread.
			CefPostTask(TID_UI, base::Bind(&RootWindowViews::OnSetTitle, this, title));
			return;
		}

		if (Window)
			Window->SetTitle(title);
	}

	void RootWindowViews::OnSetFavicon(CefRefPtr<CefImage> image)
	{
		if (!CefCurrentlyOn(TID_UI))
		{
			// Execute this method on the UI thread.
			CefPostTask(TID_UI,
				base::Bind(&RootWindowViews::OnSetFavicon, this, image));
			return;
		}

		if (Window)
			Window->SetFavicon(image);
	}

	void RootWindowViews::OnSetFullscreen(bool fullscreen)
	{
		if (!CefCurrentlyOn(TID_UI))
		{
			// Execute this method on the UI thread.
			CefPostTask(TID_UI,
				base::Bind(&RootWindowViews::OnSetFullscreen, this, fullscreen));
			return;
		}

		if (Window)
			Window->SetFullscreen(fullscreen);
	}

	void RootWindowViews::OnSetLoadingState(bool isLoading,bool canGoBack,bool canGoForward)
	{
		if (!CefCurrentlyOn(TID_UI))
		{
			// Execute this method on the UI thread.
			CefPostTask(TID_UI,
				base::Bind(&RootWindowViews::OnSetLoadingState, this, isLoading,
					canGoBack, canGoForward));
			return;
		}
	}

	void RootWindowViews::OnSetDraggableRegions(const std::vector<CefDraggableRegion>& regions)
	{
		if (!CefCurrentlyOn(TID_UI)) {
			// Execute this method on the UI thread.
			CefPostTask(TID_UI,
				base::Bind(&RootWindowViews::OnSetDraggableRegions, this, regions));
			return;
		}

		if (Window)
			Window->SetDraggableRegions(regions);
	}

	void RootWindowViews::CreateClientHandler(const std::string& url)
	{
		DCHECK(!ClientHandler);

		ClientHandler = new Cef3DHandler(this, false, url);
	}

	void RootWindowViews::InitOnMainThread(const CefBrowserSettings& settings, const std::string& startup_url)
	{
		if (!CURRENTLY_ON_MAIN_THREAD())
		{
			// Execute this method on the main thread.
			MAIN_POST_CLOSURE(
				base::Bind(&RootWindowViews::InitOnMainThread, this, settings,
					startup_url));
			return;
		}

		CreateViewsWindow(settings, startup_url, Delegate->GetRequestContext(this));
	}

	void RootWindowViews::CreateViewsWindow(const CefBrowserSettings& settings, const std::string& startup_url, CefRefPtr<CefRequestContext> request_context) 
	{
		if (!CefCurrentlyOn(TID_UI))
		{
			// Execute this method on the UI thread.
			CefPostTask(TID_UI,
				base::Bind(&RootWindowViews::CreateViewsWindow, this, settings,
					startup_url, request_context));
			return;
		}

		DCHECK(!Window);

		// Create the ViewsWindow. It will show itself after creation.
		ViewsWindow::Create(this, ClientHandler, startup_url, settings,
			request_context);
	}

	void RootWindowViews::NotifyViewsWindowDestroyed()
	{
		REQUIRE_MAIN_THREAD();
		IsWindowDestroyed = true;
		NotifyDestroyedIfDone();
	}

	void RootWindowViews::NotifyDestroyedIfDone()
	{
		// Notify once both the window and the browser have been destroyed.
		if (IsWindowDestroyed && IsBrowserDestroyed)
			Delegate->OnRootWindowDestroyed(this);
	}
}
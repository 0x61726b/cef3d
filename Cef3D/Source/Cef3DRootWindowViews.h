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

#pragma once

#include "include/base/cef_scoped_ptr.h"


namespace Cef3D
{
	class Cef3DBrowser;

	// Views framework implementation of a top-level window in the browser process.
	// The methods of this class must be called on the main thread unless otherwise
	// indicated.
	class CEF3D_API RootWindowViews : public RootWindow,
		public Cef3DHandlerDelegate,
		public ViewsWindow::Delegate
	{
	public:
		// Constructor may be called on any thread.
		RootWindowViews();
		~RootWindowViews();

		// RootWindow methods:
		void Init(RootWindow::Delegate* delegate, const Cef3DBrowserDefinition& def) OVERRIDE;

		void InitAsPopup(RootWindow::Delegate* delegate, bool with_osr, const CefPopupFeatures& popupFeatures,CefWindowInfo& windowInfo, CefRefPtr<CefClient>& client, CefBrowserSettings& settings) OVERRIDE;
		void Show(ShowMode mode) OVERRIDE;
		void Hide() OVERRIDE;
		void SetBounds(int x, int y, size_t width, size_t height) OVERRIDE;
		void Close(bool force) OVERRIDE;
		void SetDeviceScaleFactor(float device_scale_factor) OVERRIDE;
		float GetDeviceScaleFactor() const OVERRIDE;
		CefRefPtr<CefBrowser> GetBrowser() const OVERRIDE;
		WindowHandle GetWindowHandle() const OVERRIDE;

		// ViewsWindow::Delegate methods:
		CefRect GetWindowBounds() OVERRIDE;
		void OnViewsWindowCreated(CefRefPtr<ViewsWindow> window) OVERRIDE;
		void OnViewsWindowDestroyed(CefRefPtr<ViewsWindow> window) OVERRIDE;
		ViewsWindow::Delegate* GetDelegateForPopup(
			CefRefPtr<CefClient> client) OVERRIDE;
		virtual void OnExit() OVERRIDE;

	protected:
		// ClientHandler::Delegate methods:
		void OnBrowserCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
		void OnBrowserClosing(CefRefPtr<CefBrowser> browser) OVERRIDE;
		void OnBrowserClosed(CefRefPtr<CefBrowser> browser) OVERRIDE;
		void OnSetAddress(const std::string& url) OVERRIDE;
		void OnSetTitle(const std::string& title) OVERRIDE;
		void OnSetFavicon(CefRefPtr<CefImage> image) OVERRIDE;
		void OnSetFullscreen(bool fullscreen) OVERRIDE;
		void OnSetLoadingState(bool isLoading, bool canGoBack, bool canGoForward) OVERRIDE;
		void OnSetDraggableRegions(const std::vector<CefDraggableRegion>& regions) OVERRIDE;

	private:
		void CreateClientHandler(const std::string& url);

		void InitOnMainThread(const CefBrowserSettings& settings,
			const std::string& startup_url);
		void CreateViewsWindow(const CefBrowserSettings& settings,
			const std::string& startup_url,
			CefRefPtr<CefRequestContext> request_context);

		void NotifyViewsWindowDestroyed();
		void NotifyDestroyedIfDone();

		// After initialization all members are only accessed on the main thread
		// unless otherwise indicated.
		// Members set during initialization.
		RootWindow::Delegate* Delegate;
		bool IsPopup;
		bool IsOsr;

		CefRect InitialBounds;
		CefRefPtr<Cef3DHandler> ClientHandler;

		bool IsInitialized;
		bool IsWindowDestroyed;
		bool IsBrowserDestroyed;

		CefRefPtr<CefBrowser> Browser;

		// Only accessed on the browser process UI thread.
		CefRefPtr<ViewsWindow> Window;

		DISALLOW_COPY_AND_ASSIGN(RootWindowViews);
	};
}
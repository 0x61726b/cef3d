//----------------------------------------------------------------------------
// Cef3D
// Copyright (C) 2017 arkenthera
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// https://github.com/arkenthera/cef3d
// Cef3DBrowserWindow.h
// Date: 26.04.2017
//---------------------------------------------------------------------------

#pragma once

namespace Cef3D
{

	// Windows implementation of a top-level native window in the browser process.
	// The methods of this class must be called on the main thread unless otherwise
	// indicated.
	class RootWindowWin : public RootWindow,
		public BrowserWindow::Delegate {
	public:
		// Constructor may be called on any thread.
		RootWindowWin();
		~RootWindowWin();

		// RootWindow methods.
		void Init(RootWindow::Delegate* delegate,
			const Cef3DBrowserDefinition& def) OVERRIDE;
		void InitAsPopup(RootWindow::Delegate* delegate,
			bool with_osr,
			const CefPopupFeatures& popupFeatures,
			CefWindowInfo& windowInfo,
			CefRefPtr<CefClient>& client,
			CefBrowserSettings& settings) OVERRIDE;
		void Show(ShowMode mode) OVERRIDE;
		void Hide() OVERRIDE;
		void SetBounds(int x, int y, size_t width, size_t height) OVERRIDE;
		void Close(bool force) OVERRIDE;
		void SetDeviceScaleFactor(float device_scale_factor) OVERRIDE;
		float GetDeviceScaleFactor() const OVERRIDE;
		CefRefPtr<CefBrowser> GetBrowser() const OVERRIDE;
		WindowHandle GetWindowHandle() const OVERRIDE;

	private:
		void CreateBrowserWindow(const std::string& startup_url);
		void CreateRootWindow(const Cef3DBrowserDefinition& def);


		// BrowserWindow::Delegate methods.
		void OnBrowserCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
		void OnBrowserWindowDestroyed() OVERRIDE;
		void OnSetAddress(const std::string& url) OVERRIDE;
		void OnSetTitle(const std::string& title) OVERRIDE;
		void OnSetFullscreen(bool fullscreen) OVERRIDE;
		void OnSetLoadingState(bool isLoading,
			bool canGoBack,
			bool canGoForward) OVERRIDE;
		void OnSetDraggableRegions(
			const std::vector<CefDraggableRegion>& regions) OVERRIDE;

		void NotifyDestroyedIfDone();

		void OnFocus();
		void OnSize(bool minimized);
		void OnMove();
	public:
		bool OnClose();
		void OnDestroyed();
	private:

		// After initialization all members are only accessed on the main thread.
		// Members set during initialization.
		RootWindow::Delegate* delegate_;
		bool with_osr_;
		bool is_popup_;
		Cef3DRect Rect;
		scoped_ptr<BrowserWindow> browser_window_;
		bool initialized_;

		bool window_destroyed_;
		bool browser_destroyed_;

		DISALLOW_COPY_AND_ASSIGN(RootWindowWin);
	};

}
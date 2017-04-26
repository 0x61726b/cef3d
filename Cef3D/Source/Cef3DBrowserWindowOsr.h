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

	// Represents a native child window hosting a single off-screen browser
	// instance. The methods of this class must be called on the main thread unless
	// otherwise indicated.
	class BrowserWindowOsrWin : public BrowserWindow
	{
	public:
		// Constructor may be called on any thread.
		// |delegate| must outlive this object.
		BrowserWindowOsrWin(BrowserWindow::Delegate* delegate,
			const std::string& startup_url,
			const Cef3DOSRSettings& settings);

		// BrowserWindow methods.
		void CreateBrowser(WindowHandle parent_handle,
			const CefRect& rect,
			const CefBrowserSettings& settings,
			CefRefPtr<CefRequestContext> request_context) OVERRIDE;
		void GetPopupConfig(CefWindowHandle temp_handle,
			CefWindowInfo& windowInfo,
			CefRefPtr<CefClient>& client,
			CefBrowserSettings& settings) OVERRIDE;
		void ShowPopup(WindowHandle parent_handle,
			int x, int y, size_t width, size_t height) OVERRIDE;
		void Show() OVERRIDE;
		void Hide() OVERRIDE;
		void SetBounds(int x, int y, size_t width, size_t height) OVERRIDE;
		void SetFocus(bool focus) OVERRIDE;
		void SetDeviceScaleFactor(float device_scale_factor) OVERRIDE;
		float GetDeviceScaleFactor() const OVERRIDE;
		WindowHandle GetWindowHandle() const OVERRIDE;

	private:
		// ClienHandler::Delegate methods.
		void OnBrowserClosed(CefRefPtr<CefBrowser> browser) OVERRIDE;

		const bool transparent_;

		Cef3DRect client_rect;
		float device_scale_factor_;

		scoped_refptr<OsrWindowWin> osr_window_;

		DISALLOW_COPY_AND_ASSIGN(BrowserWindowOsrWin);
	};

}
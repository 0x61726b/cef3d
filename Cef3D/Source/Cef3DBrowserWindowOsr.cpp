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

#include "Cef3DPCH.h"

namespace Cef3D
{

	BrowserWindowOsrWin::BrowserWindowOsrWin(BrowserWindow::Delegate* delegate,
		const std::string& startup_url,
		const Cef3DOSRSettings& settings)
		: BrowserWindow(delegate),
		transparent_(settings.Transparent),
		device_scale_factor_(1.0f)
	{
		osr_window_ = new OsrWindowWin(settings);
		client_handler_ = new Cef3DOsrHandler(this, osr_window_, startup_url);
	}

	void BrowserWindowOsrWin::CreateBrowser(
		WindowHandle parent_handle,
		const CefRect& rect,
		const CefBrowserSettings& settings,
		CefRefPtr<CefRequestContext> request_context) {
		REQUIRE_MAIN_THREAD();

		if (osr_window_)
			osr_window_->CreateBrowser(client_handler_, settings, request_context, client_handler_->GetURL());
	}

	void BrowserWindowOsrWin::GetPopupConfig(CefWindowHandle temp_handle,
		CefWindowInfo& windowInfo,
		CefRefPtr<CefClient>& client,
		CefBrowserSettings& settings) {
		// Note: This method may be called on any thread.
		windowInfo.SetAsWindowless(temp_handle, transparent_);
		client = client_handler_;
	}

	void BrowserWindowOsrWin::ShowPopup(WindowHandle parent_handle,
		int x, int y, size_t width, size_t height) {
		REQUIRE_MAIN_THREAD();
		
		// TODO
	}

	void BrowserWindowOsrWin::Show() {
		REQUIRE_MAIN_THREAD();

		if (osr_window_)
			osr_window_->Show();
	}

	void BrowserWindowOsrWin::Hide() {
		REQUIRE_MAIN_THREAD();

		if (osr_window_)
			osr_window_->Hide();
	}

	void BrowserWindowOsrWin::SetBounds(int x, int y, size_t width, size_t height) {
		REQUIRE_MAIN_THREAD();

		if (osr_window_)
			osr_window_->SetBounds(x,y,width,height);
	}

	void BrowserWindowOsrWin::SetFocus(bool focus) {
		REQUIRE_MAIN_THREAD();

		if (osr_window_)
			osr_window_->SetFocus();
	}

	void BrowserWindowOsrWin::SetDeviceScaleFactor(float device_scale_factor) {
		REQUIRE_MAIN_THREAD();

		if (osr_window_)
			osr_window_->SetDeviceScaleFactor(device_scale_factor);
	}

	float BrowserWindowOsrWin::GetDeviceScaleFactor() const {
		REQUIRE_MAIN_THREAD();
		return device_scale_factor_;
	}

	WindowHandle BrowserWindowOsrWin::GetWindowHandle() const {
		REQUIRE_MAIN_THREAD();
		return NULL;
	}

	void BrowserWindowOsrWin::OnBrowserClosed(CefRefPtr<CefBrowser> browser) {
		REQUIRE_MAIN_THREAD();

		BrowserWindow::OnBrowserClosed(browser);
	}
}
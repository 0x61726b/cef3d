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
	BrowserWindow::BrowserWindow(Delegate* delegate)
		: delegate_(delegate),
		is_closing_(false) {
		DCHECK(delegate_);
	}

	void BrowserWindow::SetDeviceScaleFactor(float device_scale_factor) {
	}

	float BrowserWindow::GetDeviceScaleFactor() const {
		return 1.0f;
	}

	CefRefPtr<CefBrowser> BrowserWindow::GetBrowser() const {
		REQUIRE_MAIN_THREAD();
		return browser_;
	}

	bool BrowserWindow::IsClosing() const {
		REQUIRE_MAIN_THREAD();
		return is_closing_;
	}

	void BrowserWindow::OnBrowserCreated(CefRefPtr<CefBrowser> browser) {
		REQUIRE_MAIN_THREAD();
		DCHECK(!browser_);
		browser_ = browser;

		delegate_->OnBrowserCreated(browser);

		// cant close here, doesnt work on cefclient
	}

	void BrowserWindow::OnBrowserClosing(CefRefPtr<CefBrowser> browser) {
		REQUIRE_MAIN_THREAD();
		DCHECK_EQ(browser->GetIdentifier(), browser_->GetIdentifier());
		is_closing_ = true;
	}

	void BrowserWindow::OnBrowserClosed(CefRefPtr<CefBrowser> browser) {
		REQUIRE_MAIN_THREAD();
		if (browser_.get()) {
			DCHECK_EQ(browser->GetIdentifier(), browser_->GetIdentifier());
			browser_ = NULL;
		}

		client_handler_->DetachDelegate();
		client_handler_ = NULL;

		// |this| may be deleted.
		delegate_->OnBrowserWindowDestroyed();
	}

	void BrowserWindow::OnSetAddress(const std::string& url) {
		REQUIRE_MAIN_THREAD();
		delegate_->OnSetAddress(url);
	}

	void BrowserWindow::OnSetTitle(const std::string& title) {
		REQUIRE_MAIN_THREAD();
		delegate_->OnSetTitle(title);
	}

	void BrowserWindow::OnSetFullscreen(bool fullscreen) {
		REQUIRE_MAIN_THREAD();
		delegate_->OnSetFullscreen(fullscreen);
	}

	void BrowserWindow::OnSetLoadingState(bool isLoading,
		bool canGoBack,
		bool canGoForward) {
		REQUIRE_MAIN_THREAD();
		delegate_->OnSetLoadingState(isLoading, canGoBack, canGoForward);
	}

	void BrowserWindow::OnSetDraggableRegions(
		const std::vector<CefDraggableRegion>& regions) {
		REQUIRE_MAIN_THREAD();
		delegate_->OnSetDraggableRegions(regions);
	}
}
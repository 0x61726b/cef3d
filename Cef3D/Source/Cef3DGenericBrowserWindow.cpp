//----------------------------------------------------------------------------
// Cef3D
// Copyright (C) 2017 arkenthera
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// https://github.com/arkenthera/cef3d
// Cef3DOsrWindow.h
// Date: 23.04.2017
//---------------------------------------------------------------------------

#include "Cef3DPCH.h"

#include "include/base/cef_bind.h"

namespace Cef3D
{

	Cef3DGenericBrowserWindow::Cef3DGenericBrowserWindow(Cef3DGenericBrowserWindow::Delegate* delegate)
		: delegate_(delegate),
		is_closing_(false) {
		DCHECK(delegate_);
	}

	void Cef3DGenericBrowserWindow::SetDeviceScaleFactor(float device_scale_factor) {
	}

	float Cef3DGenericBrowserWindow::GetDeviceScaleFactor() const {
		return 1.0f;
	}

	CefRefPtr<CefBrowser> Cef3DGenericBrowserWindow::GetBrowser() const {
		REQUIRE_MAIN_THREAD();
		return browser_;
	}

	bool Cef3DGenericBrowserWindow::IsClosing() const {
		REQUIRE_MAIN_THREAD();
		return is_closing_;
	}

	void Cef3DGenericBrowserWindow::OnBrowserCreated(CefRefPtr<CefBrowser> browser) {
		REQUIRE_MAIN_THREAD();
		DCHECK(!browser_);
		browser_ = browser;

		//delegate_->OnBrowserCreated(browser);
	}

	void Cef3DGenericBrowserWindow::OnBrowserClosing(CefRefPtr<CefBrowser> browser) {
		REQUIRE_MAIN_THREAD();
		DCHECK_EQ(browser->GetIdentifier(), browser_->GetIdentifier());
		is_closing_ = true;
	}

	void Cef3DGenericBrowserWindow::OnBrowserClosed(CefRefPtr<CefBrowser> browser) {
		REQUIRE_MAIN_THREAD();
		if (browser_.get()) {
			DCHECK_EQ(browser->GetIdentifier(), browser_->GetIdentifier());
			browser_ = NULL;
		}

		client_handler_->DetachDelegate();
		client_handler_ = NULL;

		// |this| may be deleted.
		//delegate_->OnBrowserWindowDestroyed();
	}

	void Cef3DGenericBrowserWindow::OnSetAddress(const std::string& url) {
		REQUIRE_MAIN_THREAD();
		/*delegate_->OnSetAddress(url);*/
	}

	void Cef3DGenericBrowserWindow::OnSetTitle(const std::string& title) {
		REQUIRE_MAIN_THREAD();
		/*delegate_->OnSetTitle(title);*/
	}

	void Cef3DGenericBrowserWindow::OnSetFullscreen(bool fullscreen) {
		REQUIRE_MAIN_THREAD();
		/*delegate_->OnSetFullscreen(fullscreen);*/
	}

	void Cef3DGenericBrowserWindow::OnSetLoadingState(bool isLoading,
		bool canGoBack,
		bool canGoForward) {
		REQUIRE_MAIN_THREAD();
		/*delegate_->OnSetLoadingState(isLoading, canGoBack, canGoForward);*/
	}

	void Cef3DGenericBrowserWindow::OnSetDraggableRegions(
		const std::vector<CefDraggableRegion>& regions) {
		REQUIRE_MAIN_THREAD();
		/*delegate_->OnSetDraggableRegions(regions);*/
	}
}
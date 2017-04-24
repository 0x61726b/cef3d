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

	Cef3DOsrBrowserWindow::Cef3DOsrBrowserWindow() :
		is_closing_(false) {

	}

	void Cef3DOsrBrowserWindow::Init(const Cef3DBrowserDefinition & Def, Cef3DBrowser* browser)
	{
		std::string url = "http://google.com";
		osrBrowser_ = new Cef3DOsrBrowser(Def.Width, Def.Height, Def.PaintDelegate);
		client_handler_ = new Cef3DOsrHandler(this, osrBrowser_, url);

		CefWindowInfo windowInfo;
		windowInfo.SetAsWindowless(NULL, true);

		browser_ = 0;
		
		browser_ = CefBrowserHost::CreateBrowserSync(windowInfo, client_handler_, url, Cef3DPrivate::Cef3DBrowserDefinitionToCef(Def), 0);
		browser->SetBrowserID(browser_->GetIdentifier());
	}

	void Cef3DOsrBrowserWindow::SetBrowser(CefRefPtr<CefBrowser> browser)
	{
		browser_ = browser;
	}

	void Cef3DOsrBrowserWindow::SetDeviceScaleFactor(float device_scale_factor) {
	}

	float Cef3DOsrBrowserWindow::GetDeviceScaleFactor() const {
		return 1.0f;
	}

	CefRefPtr<CefBrowser> Cef3DOsrBrowserWindow::GetBrowser() const {
		REQUIRE_MAIN_THREAD();
		return browser_;
	}

	bool Cef3DOsrBrowserWindow::IsClosing() const {
		REQUIRE_MAIN_THREAD();
		return is_closing_;
	}

	void Cef3DOsrBrowserWindow::OnBrowserCreated(CefRefPtr<CefBrowser> browser) {
		REQUIRE_MAIN_THREAD();
		DCHECK(!browser_);
		browser_ = browser;

		//delegate_->OnBrowserCreated(browser);
	}

	void Cef3DOsrBrowserWindow::OnBrowserClosing(CefRefPtr<CefBrowser> browser) {
		REQUIRE_MAIN_THREAD();
		DCHECK_EQ(browser->GetIdentifier(), browser_->GetIdentifier());
		is_closing_ = true;
	}

	void Cef3DOsrBrowserWindow::OnBrowserClosed(CefRefPtr<CefBrowser> browser) {
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

	void Cef3DOsrBrowserWindow::OnSetAddress(const std::string& url) {
		REQUIRE_MAIN_THREAD();
		/*delegate_->OnSetAddress(url);*/
	}

	void Cef3DOsrBrowserWindow::OnSetTitle(const std::string& title) {
		REQUIRE_MAIN_THREAD();
		/*delegate_->OnSetTitle(title);*/
	}

	void Cef3DOsrBrowserWindow::OnSetFullscreen(bool fullscreen) {
		REQUIRE_MAIN_THREAD();
		/*delegate_->OnSetFullscreen(fullscreen);*/
	}

	void Cef3DOsrBrowserWindow::OnSetLoadingState(bool isLoading,
		bool canGoBack,
		bool canGoForward) {
		REQUIRE_MAIN_THREAD();
		/*delegate_->OnSetLoadingState(isLoading, canGoBack, canGoForward);*/
	}

	void Cef3DOsrBrowserWindow::OnSetDraggableRegions(
		const std::vector<CefDraggableRegion>& regions) {
		REQUIRE_MAIN_THREAD();
		/*delegate_->OnSetDraggableRegions(regions);*/
	}
}
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
	OsrWindowWin::OsrWindowWin(
		const Cef3DOSRSettings& settings)
	{
	}

	OsrWindowWin::~OsrWindowWin() {
		CEF_REQUIRE_UI_THREAD();
	}

	void OsrWindowWin::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
		CEF_REQUIRE_UI_THREAD();
		DCHECK(!browser_);
		browser_ = browser;

		
		

	}

	void OsrWindowWin::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
		CEF_REQUIRE_UI_THREAD();
		// Detach |this| from the ClientHandlerOsr.
		static_cast<Cef3DOsrHandler*>(browser_->GetHost()->GetClient().get())->
			DetachOsrDelegate();

		
		browser_ = NULL;
		//static_cast<RootWindowWin*>(GMainContext->GetRootWindowManager()->GetWindowForBrowser(browser->GetIdentifier()).get())->OnDestroyed();
	}

	bool OsrWindowWin::GetRootScreenRect(CefRefPtr<CefBrowser> browser,
		CefRect& rect) {
		CEF_REQUIRE_UI_THREAD();
		return false;
	}

	bool OsrWindowWin::GetViewRect(CefRefPtr<CefBrowser> browser,
		CefRect& rect) {
		CEF_REQUIRE_UI_THREAD();

		rect.x = rect.y = 0;
		rect.width = 800;
		rect.height = 600;

		return true;
	}

	bool OsrWindowWin::GetScreenPoint(CefRefPtr<CefBrowser> browser,
		int viewX,
		int viewY,
		int& screenX,
		int& screenY) {
		CEF_REQUIRE_UI_THREAD();


		return true;
	}

	bool OsrWindowWin::GetScreenInfo(CefRefPtr<CefBrowser> browser,
		CefScreenInfo& screen_info) {
		CEF_REQUIRE_UI_THREAD();

		CefRect view_rect;
		GetViewRect(browser, view_rect);

		screen_info.device_scale_factor = 1.0f;

		// The screen info rectangles are used by the renderer to create and position
		// popups. Keep popups inside the view rectangle.
		screen_info.rect = view_rect;
		screen_info.available_rect = view_rect;
		return true;
	}

	void OsrWindowWin::OnPopupShow(CefRefPtr<CefBrowser> browser,
		bool show) {
		CEF_REQUIRE_UI_THREAD();


	}

	void OsrWindowWin::OnPopupSize(CefRefPtr<CefBrowser> browser,
		const CefRect& rect) {
		CEF_REQUIRE_UI_THREAD();
	}

	void OsrWindowWin::OnPaint(CefRefPtr<CefBrowser> browser,
		CefRenderHandler::PaintElementType type,
		const CefRenderHandler::RectList& dirtyRects,
		const void* buffer,
		int width,
		int height) {
		CEF_REQUIRE_UI_THREAD();

		//browser->GetHost()->CloseBrowser(true);
	}

	void OsrWindowWin::OnCursorChange(
		CefRefPtr<CefBrowser> browser,
		CefCursorHandle cursor,
		CefRenderHandler::CursorType type,
		const CefCursorInfo& custom_cursor_info) {
		CEF_REQUIRE_UI_THREAD();


	}

	bool OsrWindowWin::StartDragging(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefDragData> drag_data,
		CefRenderHandler::DragOperationsMask allowed_ops,
		int x, int y) {
		CEF_REQUIRE_UI_THREAD();
		return false;
	}

	void OsrWindowWin::UpdateDragCursor(
		CefRefPtr<CefBrowser> browser,
		CefRenderHandler::DragOperation operation) {
		CEF_REQUIRE_UI_THREAD();

#if defined(CEF_USE_ATL)
		current_drag_op_ = operation;
#endif
	}

	void OsrWindowWin::OnImeCompositionRangeChanged(
		CefRefPtr<CefBrowser> browser,
		const CefRange& selection_range,
		const CefRenderHandler::RectList& character_bounds) {
		CEF_REQUIRE_UI_THREAD();


	}

	void OsrWindowWin::CreateBrowser(CefRefPtr<CefClient> handler, const CefBrowserSettings & settings, CefRefPtr<CefRequestContext> request_context, const std::string & startup_url)
	{
		if (!CefCurrentlyOn(TID_UI)) {
			// Execute this method on the UI thread.
			CefPostTask(TID_UI, base::Bind(&OsrWindowWin::CreateBrowser, this,
				handler, settings,
				request_context,startup_url));
			return;
		}

		CefWindowInfo window_info;
		window_info.SetAsWindowless(NULL, true);

		// Create the browser asynchronously.
		CefBrowserHost::CreateBrowserSync(window_info, handler, startup_url, settings,
			request_context);
	}

	void OsrWindowWin::Show() {
		if (!CefCurrentlyOn(TID_UI)) {
			// Execute this method on the UI thread.
			CefPostTask(TID_UI, base::Bind(&OsrWindowWin::Show, this));
			return;
		}

		if (!browser_)
			return;

		if (hidden_) {
			// Set the browser as visible.
			browser_->GetHost()->WasHidden(false);
			hidden_ = false;
		}

		// Give focus to the browser.
		browser_->GetHost()->SendFocusEvent(true);
	}

	void OsrWindowWin::Hide() {
		if (!CefCurrentlyOn(TID_UI)) {
			// Execute this method on the UI thread.
			CefPostTask(TID_UI, base::Bind(&OsrWindowWin::Hide, this));
			return;
		}

		if (!browser_)
			return;

		// Remove focus from the browser.
		browser_->GetHost()->SendFocusEvent(false);

		if (!hidden_) {
			// Set the browser as hidden.
			browser_->GetHost()->WasHidden(true);
			hidden_ = true;
		}
	}

	void OsrWindowWin::SetBounds(int x, int y, size_t width, size_t height) {
		if (!CefCurrentlyOn(TID_UI)) {
			// Execute this method on the UI thread.
			CefPostTask(TID_UI, base::Bind(&OsrWindowWin::SetBounds, this, x, y, width,
				height));
			return;
		}
	}

	void OsrWindowWin::SetFocus() {
		if (!CefCurrentlyOn(TID_UI)) {
			// Execute this method on the UI thread.
			CefPostTask(TID_UI, base::Bind(&OsrWindowWin::SetFocus, this));
			return;
		}

	}

	void OsrWindowWin::SetDeviceScaleFactor(float device_scale_factor) {
		if (!CefCurrentlyOn(TID_UI)) {
			// Execute this method on the UI thread.
			CefPostTask(TID_UI, base::Bind(&OsrWindowWin::SetDeviceScaleFactor, this,
				device_scale_factor));
			return;
		}

		if (device_scale_factor == device_scale_factor_)
			return;

		device_scale_factor_ = device_scale_factor;
		if (browser_) {
			browser_->GetHost()->NotifyScreenInfoChanged();
			browser_->GetHost()->WasResized();
		}
	}
}

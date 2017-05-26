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
		: renderingPopup(false),
		isPopup(settings.IsPopup)
	{
		client_rect = settings.Rect;
		IsTransparent = settings.Transparent;
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

		GMainContext->GetCef3DBrowser(browser)->OnBeforeClose();
	}

	bool OsrWindowWin::GetRootScreenRect(CefRefPtr<CefBrowser> browser,
		CefRect& rect) {
		CEF_REQUIRE_UI_THREAD();
		return false;
	}

	bool OsrWindowWin::GetViewRect(CefRefPtr<CefBrowser> browser,
		CefRect& rect) {
		CEF_REQUIRE_UI_THREAD();

		rect.x = client_rect.X;
		rect.y = client_rect.Y;
		rect.width = client_rect.Width;
		rect.height = client_rect.Height;

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

		if (!show)
		{
			popupRect.Set(0, 0, 0, 0);
			originalRect.Set(0, 0, 0, 0);

			browser->GetHost()->Invalidate(PET_VIEW);
		}

		GMainContext->GetCef3DBrowser(browser)->OnPopupShow(show);
	}

	void OsrWindowWin::OnPopupSize(CefRefPtr<CefBrowser> browser,
		const CefRect& rect) {
		CEF_REQUIRE_UI_THREAD();

		if (rect.width <= 0 || rect.height <= 0)
			return;

		originalRect = rect;
		popupRect = GetPopupRectInWebView(originalRect, client_rect.Width, client_rect.Height);
	}

	void OsrWindowWin::OnPaint(CefRefPtr<CefBrowser> browser,
		CefRenderHandler::PaintElementType type,
		const CefRenderHandler::RectList& dirtyRects,
		const void* buffer,
		int width,
		int height)
	{
		CEF_REQUIRE_UI_THREAD();

		if (renderingPopup)
		{
			std::vector<Cef3DRect> osrDirtyRectList;
			for (const CefRect& v : dirtyRects)
			{
				Cef3DRect rect;
				rect.Width = v.width;
				rect.Height = v.height;
				osrDirtyRectList.push_back(rect);
			}

			GMainContext->GetCef3DBrowser(browser)->OnPaint(Cef3DOsrRenderType::Popup, osrDirtyRectList, buffer, width, height);
			return;
		}

		Cef3DOsrRenderType osrRenderType = isPopup ? Cef3DOsrRenderType::Popup : Cef3DOsrRenderType::View;

		if (type == PET_VIEW && !popupRect.IsEmpty())
		{
			renderingPopup = true;
			browser->GetHost()->Invalidate(PET_POPUP);
			renderingPopup = false;
		}

		std::vector<Cef3DRect> osrDirtyRectList;
		for (const CefRect& v : dirtyRects)
		{
			Cef3DRect rect;
			rect.Width = v.width;
			rect.Height = v.height;
			osrDirtyRectList.push_back(rect);
		}

		GMainContext->GetCef3DBrowser(browser)->OnPaint(osrRenderType, osrDirtyRectList, buffer, width, height);
	}

	void OsrWindowWin::OnCursorChange(
		CefRefPtr<CefBrowser> browser,
		CefCursorHandle cursor,
		CefRenderHandler::CursorType type,
		const CefCursorInfo& custom_cursor_info) {
		CEF_REQUIRE_UI_THREAD();

		SetClassLongPtr(NativeHandle, GCLP_HCURSOR,
			static_cast<LONG>(reinterpret_cast<LONG_PTR>(cursor)));
		SetCursor(cursor);
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

	}

	void OsrWindowWin::OnImeCompositionRangeChanged(
		CefRefPtr<CefBrowser> browser,
		const CefRange& selection_range,
		const CefRenderHandler::RectList& character_bounds) {
		CEF_REQUIRE_UI_THREAD();


	}

	void OsrWindowWin::CreateBrowser(CefRefPtr<CefClient> handler, const Cef3DBrowserDefinition& settings, CefRefPtr<CefRequestContext> request_context, const std::string & startup_url)
	{
		if (!CefCurrentlyOn(TID_UI)) {
			// Execute this method on the UI thread.
			CefPostTask(TID_UI, base::Bind(&OsrWindowWin::CreateBrowser, this,
				handler, settings,
				request_context,startup_url));
			return;
		}

		CefBrowserSettings cefSettings = Cef3DPrivate::Cef3DBrowserDefinitionToCef(settings);

		CefWindowInfo window_info;
		window_info.SetAsWindowless(settings.ParentHandle, IsTransparent);
		
		// Create the browser asynchronously.
		CefBrowserHost::CreateBrowser(window_info, handler, startup_url, cefSettings,
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

		client_rect.X = x;
		client_rect.Y = y;
		client_rect.Width = (int)width;
		client_rect.Height = (int)height;
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
	void OsrWindowWin::ShowPopup(WindowHandle parent_handle, int x, int y, size_t width, size_t height)
	{
		if (!CefCurrentlyOn(TID_UI)) {
			// Execute this method on the UI thread.
			CefPostTask(TID_UI, base::Bind(&OsrWindowWin::ShowPopup, this,
				parent_handle, x, y, width, height));
			return;
		}

		DCHECK(browser_);
		NativeHandle = parent_handle;

		// OnPopupShow should create the native window
		GMainContext->GetCef3DBrowser(browser_)->OnPopupShow(true);
		// OnPopupShow should resize the native window
		client_rect.Width = (int)width;
		client_rect.Height = (int)height;
		client_rect.X = x;
		client_rect.Y = y;
		OnPopupSize(browser_, CefRect(x, y, (int)width, (int)height));
		browser_->GetHost()->WasResized();
	}
}

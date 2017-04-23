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

namespace Cef3D
{
	void Cef3DOsrBrowser::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
		CEF_REQUIRE_UI_THREAD();
		DCHECK(!browser_);
		browser_ = browser;
	}

	void Cef3DOsrBrowser::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
		CEF_REQUIRE_UI_THREAD();
		// Detach |this| from the ClientHandlerOsr.
		static_cast<Cef3DOsrHandler*>(browser_->GetHost()->GetClient().get())->
			DetachOsrDelegate();
		browser_ = NULL;

	}

	bool Cef3DOsrBrowser::GetRootScreenRect(CefRefPtr<CefBrowser> browser,
		CefRect& rect) {
		CEF_REQUIRE_UI_THREAD();
		return false;
	}

	bool Cef3DOsrBrowser::GetViewRect(CefRefPtr<CefBrowser> browser,
		CefRect& rect) {
		CEF_REQUIRE_UI_THREAD();

		rect.x = rect.y = 0;
		rect.width = DeviceToLogical(client_rect_.right - client_rect_.left,
			device_scale_factor_);
		rect.height = DeviceToLogical(client_rect_.bottom - client_rect_.top,
			device_scale_factor_);
		return true;
	}

	bool Cef3DOsrBrowser::GetScreenPoint(CefRefPtr<CefBrowser> browser,
		int viewX,
		int viewY,
		int& screenX,
		int& screenY) {
		CEF_REQUIRE_UI_THREAD();

		return true;
	}

	bool Cef3DOsrBrowser::GetScreenInfo(CefRefPtr<CefBrowser> browser,
		CefScreenInfo& screen_info) {
		CEF_REQUIRE_UI_THREAD();

		//CefRect view_rect;
		//GetViewRect(browser, view_rect);

		//screen_info.device_scale_factor = device_scale_factor_;

		//// The screen info rectangles are used by the renderer to create and position
		//// popups. Keep popups inside the view rectangle.
		//screen_info.rect = view_rect;
		//screen_info.available_rect = view_rect;
		return true;
	}

	void Cef3DOsrBrowser::OnPopupShow(CefRefPtr<CefBrowser> browser,
		bool show) {
		CEF_REQUIRE_UI_THREAD();

	}

	void Cef3DOsrBrowser::OnPopupSize(CefRefPtr<CefBrowser> browser,
		const CefRect& rect) {
		CEF_REQUIRE_UI_THREAD();
	}

	void Cef3DOsrBrowser::OnPaint(CefRefPtr<CefBrowser> browser,
		CefRenderHandler::PaintElementType type,
		const CefRenderHandler::RectList& dirtyRects,
		const void* buffer,
		int width,
		int height) {
		CEF_REQUIRE_UI_THREAD();

	}

	void Cef3DOsrBrowser::OnCursorChange(
		CefRefPtr<CefBrowser> browser,
		CefCursorHandle cursor,
		CefRenderHandler::CursorType type,
		const CefCursorInfo& custom_cursor_info) {
		CEF_REQUIRE_UI_THREAD();
	}

	bool Cef3DOsrBrowser::StartDragging(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefDragData> drag_data,
		CefRenderHandler::DragOperationsMask allowed_ops,
		int x, int y) {
		CEF_REQUIRE_UI_THREAD();

		return false;
	}

	void Cef3DOsrBrowser::UpdateDragCursor(
		CefRefPtr<CefBrowser> browser,
		CefRenderHandler::DragOperation operation) {
		CEF_REQUIRE_UI_THREAD();

	}

	void Cef3DOsrBrowser::OnImeCompositionRangeChanged(
		CefRefPtr<CefBrowser> browser,
		const CefRange& selection_range,
		const CefRenderHandler::RectList& character_bounds) {
		CEF_REQUIRE_UI_THREAD();

	}
	Cef3DOsrBrowser::Cef3DOsrBrowser()
	{
		client_rect_.left = 0;
		client_rect_.right = 800;
		client_rect_.top = 0;
		client_rect_.bottom = 600;

		device_scale_factor_ = 1;
	}
}
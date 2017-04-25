//----------------------------------------------------------------------------
// Cef3D
// Copyright (C) 2017 arkenthera
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// https://github.com/arkenthera/cef3d
// Cef3DOsrHandler.h
// Date: 13.04.2017
//---------------------------------------------------------------------------

#include "Cef3DPCH.h"

#include "include/base/cef_bind.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"


namespace Cef3D
{
	Cef3DOsrHandler::Cef3DOsrHandler(Cef3DHandlerDelegate* delegate,
		OsrDelegate* osr_delegate,
		const std::string& startup_url)
		: Cef3DHandler(delegate, true, startup_url),
		osr_delegate_(osr_delegate) {
		DCHECK(osr_delegate_);
	}

	void Cef3DOsrHandler::DetachOsrDelegate() {
		if (!CefCurrentlyOn(TID_UI)) {
			// Execute this method on the UI thread.
			CefPostTask(TID_UI, base::Bind(&Cef3DOsrHandler::DetachOsrDelegate, this));
			return;
		}

		DCHECK(osr_delegate_);
		osr_delegate_ = NULL;

		Cef3DHandler::DetachDelegate();
	}

	void Cef3DOsrHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
		CEF_REQUIRE_UI_THREAD();
		if (osr_delegate_)
			osr_delegate_->OnAfterCreated(browser);
		Cef3DHandler::OnAfterCreated(browser);

		
	}

	void Cef3DOsrHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
		CEF_REQUIRE_UI_THREAD();
		if (osr_delegate_)
			osr_delegate_->OnBeforeClose(browser);
		Cef3DHandler::OnBeforeClose(browser);
	}

	bool Cef3DOsrHandler::GetRootScreenRect(CefRefPtr<CefBrowser> browser,
		CefRect& rect) {
		CEF_REQUIRE_UI_THREAD();
		if (!osr_delegate_)
			return false;
		return osr_delegate_->GetRootScreenRect(browser, rect);
	}

	bool Cef3DOsrHandler::GetViewRect(CefRefPtr<CefBrowser> browser,
		CefRect& rect) {
		CEF_REQUIRE_UI_THREAD();
		if (!osr_delegate_)
			return false;
		return osr_delegate_->GetViewRect(browser, rect);
	}

	bool Cef3DOsrHandler::GetScreenPoint(CefRefPtr<CefBrowser> browser,
		int viewX,
		int viewY,
		int& screenX,
		int& screenY) {
		CEF_REQUIRE_UI_THREAD();
		if (!osr_delegate_)
			return false;
		return osr_delegate_->GetScreenPoint(browser, viewX, viewY, screenX, screenY);
	}

	bool Cef3DOsrHandler::GetScreenInfo(CefRefPtr<CefBrowser> browser,
		CefScreenInfo& screen_info) {
		CEF_REQUIRE_UI_THREAD();
		if (!osr_delegate_)
			return false;
		return osr_delegate_->GetScreenInfo(browser, screen_info);
	}

	void Cef3DOsrHandler::OnPopupShow(CefRefPtr<CefBrowser> browser,
		bool show) {
		CEF_REQUIRE_UI_THREAD();
		if (!osr_delegate_)
			return;
		return osr_delegate_->OnPopupShow(browser, show);
	}

	void Cef3DOsrHandler::OnPopupSize(CefRefPtr<CefBrowser> browser,
		const CefRect& rect) {
		CEF_REQUIRE_UI_THREAD();
		if (!osr_delegate_)
			return;
		return osr_delegate_->OnPopupSize(browser, rect);
	}

	void Cef3DOsrHandler::OnPaint(CefRefPtr<CefBrowser> browser,
		PaintElementType type,
		const RectList& dirtyRects,
		const void* buffer,
		int width,
		int height) {
		CEF_REQUIRE_UI_THREAD();
		if (!osr_delegate_)
			return;
		osr_delegate_->OnPaint(browser, type, dirtyRects, buffer, width, height);
	}

	void Cef3DOsrHandler::OnCursorChange(
		CefRefPtr<CefBrowser> browser,
		CefCursorHandle cursor,
		CursorType type,
		const CefCursorInfo& custom_cursor_info) {
		CEF_REQUIRE_UI_THREAD();
		if (!osr_delegate_)
			return;
		osr_delegate_->OnCursorChange(browser, cursor, type, custom_cursor_info);
	}

	bool Cef3DOsrHandler::StartDragging(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefDragData> drag_data,
		CefRenderHandler::DragOperationsMask allowed_ops,
		int x, int y) {
		CEF_REQUIRE_UI_THREAD();
		if (!osr_delegate_)
			return false;
		return osr_delegate_->StartDragging(browser, drag_data, allowed_ops, x, y);
	}

	void Cef3DOsrHandler::UpdateDragCursor(CefRefPtr<CefBrowser> browser,
		CefRenderHandler::DragOperation operation) {
		CEF_REQUIRE_UI_THREAD();
		if (!osr_delegate_)
			return;
		osr_delegate_->UpdateDragCursor(browser, operation);
	}

	void Cef3DOsrHandler::OnImeCompositionRangeChanged(
		CefRefPtr<CefBrowser> browser,
		const CefRange& selection_range,
		const CefRenderHandler::RectList& character_bounds) {
		CEF_REQUIRE_UI_THREAD();
		if (!osr_delegate_)
			return;
		osr_delegate_->OnImeCompositionRangeChanged(browser, selection_range,
			character_bounds);
	}
}
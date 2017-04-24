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
		GetWindow()->SetBrowser(browser);
	}

	void Cef3DOsrBrowser::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
		CEF_REQUIRE_UI_THREAD();
		// Detach |this| from the ClientHandlerOsr.
		static_cast<Cef3DOsrHandler*>(GetWindow()->GetBrowser()->GetHost()->GetClient().get())->
			DetachOsrDelegate();
		GetWindow()->SetBrowser(0);

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
		rect.width = DeviceToLogical(client_rect_.Width,
			device_scale_factor_);
		rect.height = DeviceToLogical(client_rect_.Height,
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

		DCHECK(delegate_);

		std::vector<Cef3DRect> dirtyCef3DRects;
		for (int i = 0; i < dirtyRects.size(); ++i)
			dirtyCef3DRects.push_back(CefRectToCef3D(dirtyRects[i]));

		Cef3DOsrRenderType cef3DType = Cef3DOsrRenderType::View;

		delegate_->OnPaint(GMainContext->GetCef3DBrowser(browser), cef3DType, dirtyCef3DRects, buffer, width, height);
		
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
	Cef3DOsrBrowser::Cef3DOsrBrowser(int Width,int Height,Cef3DOsrDel* del)
		: delegate_(del)
	{
		DCHECK(delegate_);
		client_rect_.Width = Width;
		client_rect_.Height = Height;
		device_scale_factor_ = 1;
	}

	Cef3DOsrBrowserWindow* Cef3DOsrBrowser::GetWindow()
	{
		DCHECK(browserWindow);
		return browserWindow;
	}
}
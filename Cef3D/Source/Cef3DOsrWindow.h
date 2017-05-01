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

	// Represents the native parent window for an off-screen browser. This object
	// must live on the CEF UI thread in order to handle CefRenderHandler callbacks.
	// The methods of this class are thread-safe unless otherwise indicated.
	class OsrWindowWin :
		public base::RefCountedThreadSafe<OsrWindowWin, CefDeleteOnUIThread>,
		public OsrDelegate
	{
	public:
		OsrWindowWin(const Cef3DOSRSettings& settings);

	private:
		friend struct CefDeleteOnThread<TID_UI>;
		friend class base::RefCountedThreadSafe<OsrWindowWin, CefDeleteOnUIThread>;

		~OsrWindowWin();
	public:

		// ClientHandlerOsr::OsrDelegate methods.
		void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
		void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
		bool GetRootScreenRect(CefRefPtr<CefBrowser> browser,
			CefRect& rect) OVERRIDE;
		bool GetViewRect(CefRefPtr<CefBrowser> browser,
			CefRect& rect) OVERRIDE;
		bool GetScreenPoint(CefRefPtr<CefBrowser> browser,
			int viewX,
			int viewY,
			int& screenX,
			int& screenY) OVERRIDE;
		bool GetScreenInfo(CefRefPtr<CefBrowser> browser,
			CefScreenInfo& screen_info) OVERRIDE;
		void OnPopupShow(CefRefPtr<CefBrowser> browser, bool show) OVERRIDE;
		void OnPopupSize(CefRefPtr<CefBrowser> browser,
			const CefRect& rect) OVERRIDE;
		void OnPaint(CefRefPtr<CefBrowser> browser,
			CefRenderHandler::PaintElementType type,
			const CefRenderHandler::RectList& dirtyRects,
			const void* buffer,
			int width,
			int height) OVERRIDE;
		void OnCursorChange(CefRefPtr<CefBrowser> browser,
			CefCursorHandle cursor,
			CefRenderHandler::CursorType type,
			const CefCursorInfo& custom_cursor_info) OVERRIDE;
		bool StartDragging(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefDragData> drag_data,
			CefRenderHandler::DragOperationsMask allowed_ops,
			int x, int y) OVERRIDE;
		void UpdateDragCursor(CefRefPtr<CefBrowser> browser,
			CefRenderHandler::DragOperation operation) OVERRIDE;
		void OnImeCompositionRangeChanged(
			CefRefPtr<CefBrowser> browser,
			const CefRange& selection_range,
			const CefRenderHandler::RectList& character_bounds) OVERRIDE;
	private:
		bool hidden_;
		float device_scale_factor_;

		CefRefPtr<CefBrowser> browser_;
		Cef3DRect client_rect;
		bool IsTransparent;

	public:
		void CreateBrowser(
			CefRefPtr<CefClient> handler,
			const Cef3DBrowserDefinition& settings,
			CefRefPtr<CefRequestContext> request_context,
			const std::string& startup_url);
		void Show();
		void Hide();
		void SetBounds(int x, int y, size_t width, size_t height);
		void SetFocus();
		void SetDeviceScaleFactor(float device_scale_factor);

		DISALLOW_COPY_AND_ASSIGN(OsrWindowWin);
	};
}
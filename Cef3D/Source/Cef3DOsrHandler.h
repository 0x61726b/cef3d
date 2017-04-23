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

#pragma once

#include "Cef3DOsrWindow.h"

namespace Cef3D
{
	// Client handler implementation for windowless browsers. There will only ever
	// be one browser per handler instance.
	class Cef3DOsrHandler : public Cef3DHandler,
		public CefRenderHandler
	{
	public:
		Cef3DOsrHandler(Cef3DHandlerDelegate* delegate,
			OsrDelegate* osr_delegate,
			const std::string& startup_url);

		// This object may outlive the OsrDelegate object so it's necessary for the
		// OsrDelegate to detach itself before destruction.
		void DetachOsrDelegate();

		// CefClient methods.
		CefRefPtr<CefRenderHandler> GetRenderHandler() OVERRIDE {
			return this;
		}

		// CefLifeSpanHandler methods.
		void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
		void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

		// CefRenderHandler methods.
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
			CursorType type,
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
		// Only accessed on the UI thread.
		OsrDelegate* osr_delegate_;

		// Include the default reference counting implementation.
		IMPLEMENT_REFCOUNTING(Cef3DOsrHandler);
		DISALLOW_COPY_AND_ASSIGN(Cef3DOsrHandler);
	};
}
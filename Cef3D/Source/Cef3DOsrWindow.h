//----------------------------------------------------------------------------
// Cef3D
// Copyright (C) 2017 arkenthera
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// https://github.com/arkenthera/cef3d
// Cef3DOsrWindow.h
// Date: 13.04.2017
//---------------------------------------------------------------------------

#pragma once

#include "include/cef_render_handler.h"

namespace Cef3D
{
	class OsrDelegate
	{
	public:
		// These methods match the CefLifeSpanHandler interface.
		virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) = 0;
		virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) = 0;

		// These methods match the CefRenderHandler interface.
		virtual bool GetRootScreenRect(CefRefPtr<CefBrowser> browser,
			CefRect& rect) = 0;
		virtual bool GetViewRect(CefRefPtr<CefBrowser> browser,
			CefRect& rect) = 0;
		virtual bool GetScreenPoint(CefRefPtr<CefBrowser> browser,
			int viewX,
			int viewY,
			int& screenX,
			int& screenY) = 0;
		virtual bool GetScreenInfo(CefRefPtr<CefBrowser> browser,
			CefScreenInfo& screen_info) = 0;
		virtual void OnPopupShow(CefRefPtr<CefBrowser> browser, bool show) = 0;
		virtual void OnPopupSize(CefRefPtr<CefBrowser> browser,
			const CefRect& rect) = 0;
		virtual void OnPaint(CefRefPtr<CefBrowser> browser,
			CefRenderHandler::PaintElementType type,
			const CefRenderHandler::RectList& dirtyRects,
			const void* buffer,
			int width,
			int height) = 0;
		virtual void OnCursorChange(
			CefRefPtr<CefBrowser> browser,
			CefCursorHandle cursor,
			CefRenderHandler::CursorType type,
			const CefCursorInfo& custom_cursor_info) = 0;
		virtual bool StartDragging(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefDragData> drag_data,
			CefRenderHandler::DragOperationsMask allowed_ops,
			int x, int y) = 0;
		virtual void UpdateDragCursor(
			CefRefPtr<CefBrowser> browser,
			CefRenderHandler::DragOperation operation) = 0;
		virtual void OnImeCompositionRangeChanged(
			CefRefPtr<CefBrowser> browser,
			const CefRange& selection_range,
			const CefRenderHandler::RectList& character_bounds) = 0;

	protected:
		virtual ~OsrDelegate() {}
	};

	class Cef3DOsrDel
	{
	public:
		virtual void OnAfterCreated(Cef3DBrowser* browser) = 0;
		virtual void OnBeforeClose(Cef3DBrowser* browser) = 0;

		virtual bool GetViewRect(Cef3DBrowser* browser,
			Cef3DRect& rect) = 0;

		virtual void OnPaint(Cef3DBrowser* browser,
			Cef3D::Cef3DOsrRenderType type,
			const std::vector<Cef3DRect>& dirtyRects,
			const void* buffer,
			int width,
			int height) = 0;

	protected:
		virtual ~Cef3DOsrDel() {}

	};

	class Cef3DOsrBrowser
		: public Cef3D::OsrDelegate
	{
	public:
		Cef3DOsrBrowser(int Width, int Height, Cef3DOsrDel* Delegate);


		//OsrDelegete methods
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
		CefRefPtr<CefBrowser> browser_;
		Cef3DRect client_rect_;
		Cef3DOsrDel* delegate_;
		float device_scale_factor_;
	};
}
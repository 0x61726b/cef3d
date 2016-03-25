// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_TESTS_CEFSIMPLE_SIMPLE_HANDLER_H_
#define CEF_TESTS_CEFSIMPLE_SIMPLE_HANDLER_H_

#include "include/cef_client.h"

#include <list>

class CefOsrDelegate;

class SimpleHandler : public CefClient,
	public CefLifeSpanHandler,
	public CefRenderHandler {
public:
	SimpleHandler(CefOsrDelegate* del);
	~SimpleHandler();

	// Provide access to the single global instance of this object.
	static SimpleHandler* GetInstance();

	// CefClient methods:
	virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE{
		return this;
	}
		virtual CefRefPtr<CefRenderHandler> GetRenderHandler() {
		return this;
	}

	// CefLifeSpanHandler methods:
	virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
	virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
	virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

	void HandleKeyDown(unsigned key,unsigned mouseButton,int repeat);
	void HandleKeyUp(unsigned key,unsigned mouseButton,int repeat);
	void HandleMouseButtonDown(int mouseX,int mouseY,int modifiers,int button,unsigned buttons);
	void HandleMouseButtonUp(int mouseX,int mouseY,int modifiers,int button,unsigned buttons);
	void HandleMouseMove(int mouseX,int mouseY,int modifiers,unsigned buttons);

	void HandleKeyEvent(int type,int modifiers,unsigned key);


	//CefRenderHandler
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
	void OnPopupShow(CefRefPtr<CefBrowser> browser,bool show) OVERRIDE;
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
		int x,int y) OVERRIDE;
	void UpdateDragCursor(CefRefPtr<CefBrowser> browser,
		CefRenderHandler::DragOperation operation) OVERRIDE;

	// Request that all existing browser windows close.
	void CloseAllBrowsers(bool force_close);

	void AddBrowserRect(const CefRect& rect);

	int GetBrowserListSize() const { return browser_list_.size(); }

	bool IsClosing() const { return is_closing_; }

private:
	// List of existing browser windows. Only accessed on the CEF UI thread.
	typedef std::list<CefRefPtr<CefBrowser> > BrowserList;
	BrowserList browser_list_;

	typedef std::vector<CefRect> BrowserRectList;
	BrowserRectList browser_rects_;

	CefOsrDelegate* osr_delegate_;

	bool is_closing_;

	// Include the default reference counting implementation.
	IMPLEMENT_REFCOUNTING(SimpleHandler);
};

#endif  // CEF_TESTS_CEFSIMPLE_SIMPLE_HANDLER_H_

// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef __Cef3DClientHandler_h__
#define __Cef3DClientHandler_h__

#include "include/cef_client.h"

#include <list>
#include <set>

class CefOsrDelegate;

class Cef3DClientHandler :
	public CefClient,
	public CefLifeSpanHandler,
	public CefRenderHandler,
	public CefLoadHandler
{
public:
	Cef3DClientHandler(CefOsrDelegate* del);
	~Cef3DClientHandler();

	class ProcessMessageDelegate : public virtual CefBase {
	public:
		explicit ProcessMessageDelegate(const char* message_namespace)
			: message_namespace_(message_namespace) {}

		// Called when a process message is received. Return true if the message was
		// handled and should not be passed on to other handlers.
		// ProcessMessageDelegates should check for unique message names to avoid
		// interfering with each other.
		virtual bool OnProcessMessageReceived(
			CefRefPtr<Cef3DClientHandler> handler,
			CefRefPtr<CefBrowser> browser,
			CefProcessId source_process,
			CefRefPtr<CefProcessMessage> message) {
			return false;
		}

		virtual bool IsAcceptedNamespace(std::string message_name) {
			return (
				message_namespace_
				&& message_name.find(message_namespace_) == 0);
		}

	protected:
		const char* message_namespace_;

	};

	void SendJSEvent(int browserIndex, const CefString& name,const CefString& data);
	typedef std::set<CefRefPtr<ProcessMessageDelegate> >
		ProcessMessageDelegateSet;

	// Provide access to the single global instance of this object.
	static Cef3DClientHandler* GetInstance();

	// CefClient methods:
	virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE{
		return this;
	}
		virtual CefRefPtr<CefRenderHandler> GetRenderHandler() {
		return this;
	}

	virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
		CefProcessId source_process,
		CefRefPtr<CefProcessMessage> message)
		OVERRIDE;

	// CefLifeSpanHandler methods:
	virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
	virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
	virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

	//CefLoadHandler
	void OnLoadStart(CefRefPtr<CefBrowser> browser,
                           CefRefPtr<CefFrame> frame);

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

	CefRefPtr<CefBrowser> GetOverlayBrowser() { return browser_list_.front(); }

private:
	// List of existing browser windows. Only accessed on the CEF UI thread.
	typedef std::list<CefRefPtr<CefBrowser> > BrowserList;
	BrowserList browser_list_;

	ProcessMessageDelegateSet process_message_delegates_;

	typedef std::vector<CefRect> BrowserRectList;
	BrowserRectList browser_rects_;

	CefOsrDelegate* osr_delegate_;

	bool is_closing_;

	// Include the default reference counting implementation.
	IMPLEMENT_REFCOUNTING(Cef3DClientHandler);
};

#endif  // CEF_TESTS_CEFSIMPLE_SIMPLE_HANDLER_H_

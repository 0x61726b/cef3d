// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "simple_handler.h"

#include <sstream>
#include <string>

#include "include/base/cef_bind.h"
#include "include/cef_app.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"

#include "Cef3D.h"
namespace {

	SimpleHandler* g_instance = NULL;

}  // namespace

SimpleHandler::SimpleHandler(CefOsrDelegate* del)
	: is_closing_(false),osr_delegate_(del) {
	DCHECK(!g_instance);
	g_instance = this;
}

SimpleHandler::~SimpleHandler() {
	g_instance = NULL;
}

// static
SimpleHandler* SimpleHandler::GetInstance() {
	return g_instance;
}

void SimpleHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
	CEF_REQUIRE_UI_THREAD();

	// Add to the list of existing browsers.
	browser_list_.push_back(browser);
	osr_delegate_->OnBrowserReady(browser_list_.size() - 1);
}

bool SimpleHandler::DoClose(CefRefPtr<CefBrowser> browser) {
	CEF_REQUIRE_UI_THREAD();

	// Closing the main window requires special handling. See the DoClose()
	// documentation in the CEF header for a detailed destription of this
	// process.
	if(browser_list_.size() == 1) {
		// Set a flag to indicate that the window close should be allowed.
		is_closing_ = true;
	}

	// Allow the close. For windowed browsers this will result in the OS close
	// event being sent.
	return false;
}

void SimpleHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
	CEF_REQUIRE_UI_THREAD();

	// Remove from the list of existing browsers.
	BrowserList::iterator bit = browser_list_.begin();
	for(; bit != browser_list_.end(); ++bit) {
		if((*bit)->IsSame(browser)) {
			browser_list_.erase(bit);
			break;
		}
	}
}

void SimpleHandler::CloseAllBrowsers(bool force_close) {
	if(!CefCurrentlyOn(TID_UI)) {
		// Execute on the UI thread.
		CefPostTask(TID_UI,
			base::Bind(&SimpleHandler::CloseAllBrowsers,this,force_close));
		return;
	}

	if(browser_list_.empty())
		return;

	BrowserList::const_iterator it = browser_list_.begin();
	for(; it != browser_list_.end(); ++it)
		(*it)->GetHost()->CloseBrowser(force_close);
}


bool SimpleHandler::GetRootScreenRect(CefRefPtr<CefBrowser> browser,
	CefRect& rect) {
	CEF_REQUIRE_UI_THREAD();
	return true;
}

bool SimpleHandler::GetViewRect(CefRefPtr<CefBrowser> browser,
	CefRect& rect) {
	CEF_REQUIRE_UI_THREAD();

	int browserIndex = 0;
	if(browser_list_.size() > 1)
	{
		BrowserList::iterator bit = browser_list_.begin();
		for(; bit != browser_list_.end(); ++bit) {
			if((*bit)->IsSame(browser)) {
				break;
			}
			browserIndex++;
		}
	}
	rect = browser_rects_[browserIndex];
	return true;
}

void SimpleHandler::AddBrowserRect(const CefRect& rect)
{
	browser_rects_.push_back(rect);
}

bool SimpleHandler::GetScreenPoint(CefRefPtr<CefBrowser> browser,
	int viewX,
	int viewY,
	int& screenX,
	int& screenY) {
	CEF_REQUIRE_UI_THREAD();

	return true;
}

bool SimpleHandler::GetScreenInfo(CefRefPtr<CefBrowser> browser,
	CefScreenInfo& screen_info) {
	CEF_REQUIRE_UI_THREAD();
	return true;
}

void SimpleHandler::OnPopupShow(CefRefPtr<CefBrowser> browser,
	bool show) {
	CEF_REQUIRE_UI_THREAD();

}

void SimpleHandler::OnPopupSize(CefRefPtr<CefBrowser> browser,
	const CefRect& rect) {
	CEF_REQUIRE_UI_THREAD();

}

void SimpleHandler::OnPaint(CefRefPtr<CefBrowser> browser,
	PaintElementType type,
	const RectList& dirtyRects,
	const void* buffer,
	int width,
	int height) {
	CEF_REQUIRE_UI_THREAD();

	if(osr_delegate_)
	{
		int browserIndex = 0;
		if(browser_list_.size() > 1)
		{

			BrowserList::iterator bit = browser_list_.begin();
			for(; bit != browser_list_.end(); ++bit) {
				if((*bit)->IsSame(browser)) {
					break;
				}
				browserIndex++;
			}
		}
		std::vector<CefUI::CefRect> rects;
		RectList::const_iterator rectIt = dirtyRects.begin();

		for(; rectIt != dirtyRects.end(); ++rectIt)
		{
			CefRect rect = (*rectIt);
			rects.push_back(CefUI::CefRect(rect.x,rect.width,rect.y,rect.height));
		}
		osr_delegate_->OnPaint(browserIndex,rects,buffer,width,height);
	}

}

void SimpleHandler::OnCursorChange(
	CefRefPtr<CefBrowser> browser,
	CefCursorHandle cursor,
	CursorType type,
	const CefCursorInfo& custom_cursor_info) {
	CEF_REQUIRE_UI_THREAD();

}

bool SimpleHandler::StartDragging(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefDragData> drag_data,
	CefRenderHandler::DragOperationsMask allowed_ops,
	int x,int y) {
	CEF_REQUIRE_UI_THREAD();
	return true;
}

void SimpleHandler::UpdateDragCursor(CefRefPtr<CefBrowser> browser,
	CefRenderHandler::DragOperation operation) {
	CEF_REQUIRE_UI_THREAD();
}

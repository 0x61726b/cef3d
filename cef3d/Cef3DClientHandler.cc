// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "Cef3DClientHandler.h"

#include <sstream>
#include <string>

#include "include/base/cef_bind.h"
#include "include/cef_app.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"

#include "Cef3DBrowserDelegate.h"
#include "Cef3D.h"
namespace {

	Cef3DClientHandler* g_instance = NULL;

}  // namespace

Cef3DClientHandler::Cef3DClientHandler(CefOsrDelegate* del)
	: is_closing_(false),osr_delegate_(del) {
	DCHECK(!g_instance);
	g_instance = this;

	CefUI::Cef3DBrowserDelegate::CreateProcessMessageDelegates(&process_message_delegates_);
}

Cef3DClientHandler::~Cef3DClientHandler() {
	g_instance = NULL;
}

// static
Cef3DClientHandler* Cef3DClientHandler::GetInstance() {
	return g_instance;
}

void Cef3DClientHandler::OnLoadStart(CefRefPtr<CefBrowser> browser,
                           CefRefPtr<CefFrame> frame)
{

}

bool Cef3DClientHandler::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
	CefProcessId source_process,
	CefRefPtr<CefProcessMessage> message)
{
	std::string message_name = message->GetName();
	bool handled = false;
	// Execute delegate callbacks.
	for(const auto &delegate : process_message_delegates_) {
		handled = delegate->OnProcessMessageReceived(
			this,
			browser,
			source_process,
			message
			);
	}

	if(!handled)
		LOG(WARNING) << "Unknown proccess message: " << message_name;

	return handled;
}

void Cef3DClientHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
	CEF_REQUIRE_UI_THREAD();

	// Add to the list of existing browsers.
	browser_list_.push_back(browser);
	osr_delegate_->OnBrowserReady(browser_list_.size() - 1);
}

bool Cef3DClientHandler::DoClose(CefRefPtr<CefBrowser> browser) {
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

void Cef3DClientHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
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

void Cef3DClientHandler::CloseAllBrowsers(bool force_close) {
	if(!CefCurrentlyOn(TID_UI)) {
		// Execute on the UI thread.
		CefPostTask(TID_UI,
			base::Bind(&Cef3DClientHandler::CloseAllBrowsers,this,force_close));
		return;
	}

	if(browser_list_.empty())
		return;

	BrowserList::const_iterator it = browser_list_.begin();
	for(; it != browser_list_.end(); ++it)
		(*it)->GetHost()->CloseBrowser(force_close);
}

void Cef3DClientHandler::SendJSEvent(int browserIndex,const CefString& name,const CefString& data)
{
	CefRefPtr<CefBrowser> browser;
	int counter = 0;
	BrowserList::const_iterator it = browser_list_.begin();
	for(; it != browser_list_.end(); ++it)
	{
		if(counter == browserIndex)
			browser = *it;
	}

	if(!browser)
		return;

	CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("dispatchEvent");
	message->GetArgumentList()->SetString(0,name);
	message->GetArgumentList()->SetString(1,data);


	browser->SendProcessMessage(PID_RENDERER,message);
}


void Cef3DClientHandler::HandleKeyEvent(int type,int modifiers,unsigned key)
{
	CefKeyEvent event;


	if(key == 1073742049)
		key = 16;
	if(key == 1073742048)
		key = 17;
	if(key	== 1073742050)
		key = 18;

	event.windows_key_code = key;

	if(type == 0)
		event.type = KEYEVENT_RAWKEYDOWN;
	if(type == 1)
		event.type = KEYEVENT_CHAR;
	if(type == 2)
		event.type = KEYEVENT_KEYUP;


	event.modifiers = modifiers;

	BrowserList::const_iterator it = browser_list_.begin();
	for(; it != browser_list_.end(); ++it)
		(*it)->GetHost()->SendKeyEvent(event);

}

void Cef3DClientHandler::HandleKeyDown(unsigned key,unsigned mouseButton,int repeat)
{
	CefKeyEvent event;
	event.windows_key_code = key;
	event.native_key_code = key;

	event.type = KEYEVENT_RAWKEYDOWN;
	BrowserList::const_iterator it = browser_list_.begin();
	for(; it != browser_list_.end(); ++it)
		(*it)->GetHost()->SendKeyEvent(event);
}


void Cef3DClientHandler::HandleKeyUp(unsigned key,unsigned mouseButton,int repeat)
{
	CefKeyEvent event;
	event.native_key_code = key;
	event.windows_key_code = key;
	event.type = KEYEVENT_KEYUP;
	BrowserList::const_iterator it = browser_list_.begin();
	for(; it != browser_list_.end(); ++it)
		(*it)->GetHost()->SendKeyEvent(event);
}

void Cef3DClientHandler::HandleMouseButtonDown(int mouseX,int mouseY,int native,int button,unsigned buttons)
{
	CefMouseEvent event;
	event.x = mouseX;
	event.y = mouseY;
	event.modifiers = native;

	bool mouseUp = false;
	CefBrowserHost::MouseButtonType btnType = MBT_LEFT;
	BrowserList::const_iterator it = browser_list_.begin();
	for(; it != browser_list_.end(); ++it)
		(*it)->GetHost()->SendMouseClickEvent(event,btnType,mouseUp,1);
}

void Cef3DClientHandler::HandleMouseButtonUp(int mouseX,int mouseY,int native,int button,unsigned buttons)
{
	CefMouseEvent event;
	event.x = mouseX;
	event.y = mouseY;
	event.modifiers = native;

	bool mouseUp = true;
	CefBrowserHost::MouseButtonType btnType = MBT_LEFT;
	BrowserList::const_iterator it = browser_list_.begin();
	for(; it != browser_list_.end(); ++it)
		(*it)->GetHost()->SendMouseClickEvent(event,btnType,mouseUp,1);
}

void Cef3DClientHandler::HandleMouseMove(int mouseX,int mouseY,int native,unsigned buttons)
{
	CefMouseEvent event;
	event.x = mouseX;
	event.y = mouseY;
	event.modifiers = native;

	BrowserList::const_iterator it = browser_list_.begin();
	for(; it != browser_list_.end(); ++it)
		(*it)->GetHost()->SendMouseMoveEvent(event,false);

}


bool Cef3DClientHandler::GetRootScreenRect(CefRefPtr<CefBrowser> browser,
	CefRect& rect) {
	CEF_REQUIRE_UI_THREAD();
	return true;
}

bool Cef3DClientHandler::GetViewRect(CefRefPtr<CefBrowser> browser,
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

void Cef3DClientHandler::AddBrowserRect(const CefRect& rect)
{
	browser_rects_.push_back(rect);
}

bool Cef3DClientHandler::GetScreenPoint(CefRefPtr<CefBrowser> browser,
	int viewX,
	int viewY,
	int& screenX,
	int& screenY) {
	CEF_REQUIRE_UI_THREAD();

	return true;
}

bool Cef3DClientHandler::GetScreenInfo(CefRefPtr<CefBrowser> browser,
	CefScreenInfo& screen_info) {
	CEF_REQUIRE_UI_THREAD();
	return true;
}

void Cef3DClientHandler::OnPopupShow(CefRefPtr<CefBrowser> browser,
	bool show) {
	CEF_REQUIRE_UI_THREAD();

}

void Cef3DClientHandler::OnPopupSize(CefRefPtr<CefBrowser> browser,
	const CefRect& rect) {
	CEF_REQUIRE_UI_THREAD();

}

void Cef3DClientHandler::OnPaint(CefRefPtr<CefBrowser> browser,
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

void Cef3DClientHandler::OnCursorChange(
	CefRefPtr<CefBrowser> browser,
	CefCursorHandle cursor,
	CursorType type,
	const CefCursorInfo& custom_cursor_info) {
	CEF_REQUIRE_UI_THREAD();

}

bool Cef3DClientHandler::StartDragging(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefDragData> drag_data,
	CefRenderHandler::DragOperationsMask allowed_ops,
	int x,int y) {
	CEF_REQUIRE_UI_THREAD();
	return true;
}

void Cef3DClientHandler::UpdateDragCursor(CefRefPtr<CefBrowser> browser,
	CefRenderHandler::DragOperation operation) {
	CEF_REQUIRE_UI_THREAD();
}

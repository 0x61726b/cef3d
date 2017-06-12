//----------------------------------------------------------------------------
// Cef3D
// Copyright (C) 2017 arkenthera
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// https://github.com/arkenthera/cef3d
// Cef3D.h
// Date: 18.04.2017
//---------------------------------------------------------------------------

#include "Cef3DPCH.h"

#include "include/cef_frame.h"

namespace Cef3D
{
	Cef3DBrowser::Cef3DBrowser()
		: BrowserID(-1)
	{

	}

	void Cef3DBrowser::Invalidate()
	{
		AssociatedWindow->GetBrowser()->GetHost()->Invalidate(PET_VIEW);
	}

	void Cef3DBrowser::SendMouseClickEvent(Cef3DMouseEventType type, int x, int y, unsigned modifiers,int clickCount)
	{
		DCHECK(AssociatedWindow);

		CefRefPtr<CefBrowserHost> host = AssociatedWindow->GetBrowser()->GetHost();
		switch (type)
		{
		case CEF3D_LBUTTON_DOWN:
		case CEF3D_RBUTTON_DOWN:
		case CEF3D_MBUTTON_DOWN:
		{
			CefMouseEvent mouse_event;
			mouse_event.x = x;
			mouse_event.y = y;
			mouse_event.modifiers = modifiers;

			CefBrowserHost::MouseButtonType btnType =
				(type == CEF3D_LBUTTON_DOWN ? MBT_LEFT : (
					type == CEF3D_RBUTTON_DOWN ? MBT_RIGHT : MBT_MIDDLE));

			host->SendMouseClickEvent(mouse_event, btnType, false,
				clickCount);
			break;
		}
		case CEF3D_LBUTTON_UP:
		case CEF3D_RBUTTON_UP:
		case CEF3D_MBUTTON_UP:
		{
			CefMouseEvent mouse_event;
			mouse_event.x = x;
			mouse_event.y = y;
			mouse_event.modifiers = modifiers;

			CefBrowserHost::MouseButtonType btnType =
				(type == CEF3D_LBUTTON_UP ? MBT_LEFT : (
					type == CEF3D_RBUTTON_UP ? MBT_RIGHT : MBT_MIDDLE));

			host->SendMouseClickEvent(mouse_event, btnType, true,
				clickCount);
			break;
		}
		case CEF3D_MOUSE_MOVE:
		{
			CefMouseEvent mouse_event;
			mouse_event.x = x;
			mouse_event.y = y;
			mouse_event.modifiers = modifiers;

			host->SendMouseMoveEvent(mouse_event, false);
			break;
		}
		case CEF3D_MOUSE_LEAVE:
		{
			CefMouseEvent mouse_event;
			mouse_event.x = x;
			mouse_event.y = y;
			mouse_event.modifiers = modifiers;

			host->SendMouseMoveEvent(mouse_event, true);
			break;
		}
		break;
		default:
			break;
		}
	}
	
	void Cef3DBrowser::SendMouseWheelEvent(int x, int y, int deltaX, int deltaY, unsigned modifiers)
	{
		DCHECK(AssociatedWindow);

		CefRefPtr<CefBrowserHost> host = AssociatedWindow->GetBrowser()->GetHost();

		CefMouseEvent mouse_event;
		mouse_event.x = x;
		mouse_event.y = y;
		mouse_event.modifiers = modifiers;

		host->SendMouseWheelEvent(mouse_event, deltaX, deltaY);
	}

	void Cef3DBrowser::SendKeyEvent(Cef3DKeyEventType type, int native_key, int windows_key_code,bool isSystem, unsigned modifiers)
	{
		CefKeyEvent event;
		event.windows_key_code = windows_key_code;
		event.native_key_code = native_key;
		event.is_system_key = isSystem;
		event.modifiers = modifiers;

		switch (type)
		{
		case CEF3D_KEY_RAWKEYDOWN:
			event.type = KEYEVENT_RAWKEYDOWN;
			break;
		case CEF3D_KEY_UP:
			event.type = KEYEVENT_KEYUP;
			break;
		case CEF3D_KEY_CHAR:
			event.type = KEYEVENT_CHAR;
			break;
		default:
			DCHECK(0);
			break;
		}

		AssociatedWindow->GetBrowser()->GetHost()->SendKeyEvent(event);
	}
	
	void Cef3DBrowser::SendResize(const Cef3DRect& newRect)
	{
		AssociatedWindow->SetBounds(newRect.X, newRect.Y, newRect.Width, newRect.Height);
		
		AssociatedWindow->GetBrowser()->GetHost()->WasResized();
	}
	void Cef3DBrowser::LoadURL(const std::string & url)
	{
		Url = url;
		AssociatedWindow->GetBrowser()->GetMainFrame()->LoadURL(Url);
	}

	void Cef3DBrowser::Close(bool force)
	{
		AssociatedWindow->GetBrowser()->GetHost()->CloseBrowser(force);
	}

	bool Cef3DBrowser::OnClose()
	{
		return ((RootWindowWin*)AssociatedWindow)->OnClose();
	}

	void Cef3DBrowser::Back()
	{
		GMainContext->GetCefBrowser(BrowserID)->GoBack();
	}

	void Cef3DBrowser::Forward()
	{
		GMainContext->GetCefBrowser(BrowserID)->GoForward();
	}

	void Cef3DBrowser::Stop()
	{
		GMainContext->GetCefBrowser(BrowserID)->StopLoad();
	}

	void Cef3DBrowser::Reload()
	{
		GMainContext->GetCefBrowser(BrowserID)->Reload();
	}

	void Cef3DBrowser::ReloadIgnoreCache()
	{
		GMainContext->GetCefBrowser(BrowserID)->ReloadIgnoreCache();
	}

	void Cef3DBrowser::Focus()
	{
		GMainContext->GetCefBrowser(BrowserID)->GetHost()->SetFocus(true);
	}

	void Cef3DBrowser::Blur()
	{
		GMainContext->GetCefBrowser(BrowserID)->GetHost()->SetFocus(false);
	}

	void Cef3DBrowser::PauseRendering()
	{

	}

	void Cef3DBrowser::ResumeRendering()
	{
	}

	void Cef3DBrowser::Zoom()
	{
		GMainContext->GetCefBrowser(BrowserID)->GetHost()->SetZoomLevel(20);
	}

	void Cef3DBrowser::ZoomOut()
	{
		GMainContext->GetCefBrowser(BrowserID)->GetHost()->SetZoomLevel(-20);
	}

	void Cef3DBrowser::SetZoom(int Percent)
	{
		GMainContext->GetCefBrowser(BrowserID)->GetHost()->SetZoomLevel(Percent);
	}

	void Cef3DBrowser::SetWidth(int W)
	{

	}

	void Cef3DBrowser::SetHeight(int H)
	{
	}

	void Cef3DBrowser::Undo()
	{
		GMainContext->GetCefBrowser(BrowserID)->GetFocusedFrame()->Undo();
	}

	void Cef3DBrowser::Redo()
	{
		GMainContext->GetCefBrowser(BrowserID)->GetFocusedFrame()->Redo();
	}

	void Cef3DBrowser::Cut()
	{
		GMainContext->GetCefBrowser(BrowserID)->GetFocusedFrame()->Cut();
	}

	void Cef3DBrowser::Copy()
	{
		GMainContext->GetCefBrowser(BrowserID)->GetFocusedFrame()->Copy();
	}

	void Cef3DBrowser::Paste()
	{
		GMainContext->GetCefBrowser(BrowserID)->GetFocusedFrame()->Paste();
	}

	void Cef3DBrowser::SelectAll()
	{
		GMainContext->GetCefBrowser(BrowserID)->GetFocusedFrame()->SelectAll();
	}

	double Cef3DBrowser::GetZoomLevel()
	{
		return GMainContext->GetCefBrowser(BrowserID)->GetHost()->GetZoomLevel();
	}
	bool Cef3DBrowser::CanGoBack()
	{
		return GMainContext->GetCefBrowser(BrowserID)->CanGoBack();
	}

	bool Cef3DBrowser::CanGoForward()
	{
		return GMainContext->GetCefBrowser(BrowserID)->CanGoForward();
	}

	bool Cef3DBrowser::IsLoading()
	{
		return GMainContext->GetCefBrowser(BrowserID)->IsLoading();
	}

	bool Cef3DBrowser::IsCrashed()
	{
		return false;
	}

	void Cef3DBrowser::OnProcessMessageReceived(Cef3DIPCMessageType type, const Cef3DIPCMessageResult & result)
	{
	}

	void Cef3DBrowser::CreateJsObject(Cef3DJsInt & value)
	{
		CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("create_object_request");
		CefRefPtr<CefListValue> message_list = message->GetArgumentList();

		message_list->SetInt(0, value.GetType());
		message_list->SetString(1, value.GetName());
		message_list->SetInt(2, value.GetValue());
		AssociatedWindow->GetBrowser()->SendProcessMessage(PID_RENDERER, message);
	}

	void Cef3DBrowser::CreateJsObject(Cef3DJsString & value)
	{
		CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("create_object_request");
		CefRefPtr<CefListValue> message_list = message->GetArgumentList();

		message_list->SetInt(0, value.GetType());
		message_list->SetString(1, value.GetName());
		message_list->SetString(2, value.GetValue());
		AssociatedWindow->GetBrowser()->SendProcessMessage(PID_RENDERER, message);
	}

	void Cef3DBrowser::SetJsObjectValue(Cef3DJsInt& obj, Cef3DJsInt& value)
	{
		CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("set_object_value");
		CefRefPtr<CefListValue> message_list = message->GetArgumentList();

		message_list->SetInt(0, value.GetType());
		message_list->SetString(1, value.GetName());
		message_list->SetInt(2, value.GetValue());

		AssociatedWindow->GetBrowser()->SendProcessMessage(PID_RENDERER, message);
	}

	void Cef3DBrowser::SetJsObjectValue(Cef3DJsString& obj, Cef3DJsString& value)
	{
		CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("set_object_value");
		CefRefPtr<CefListValue> message_list = message->GetArgumentList();

		message_list->SetInt(0, value.GetType());
		message_list->SetString(1, value.GetName());
		message_list->SetString(2, value.GetValue());
		AssociatedWindow->GetBrowser()->SendProcessMessage(PID_RENDERER, message);
	}

	void Cef3DBrowser::SetBrowserID(int ID)
	{
		BrowserID = ID;
	}

	Cef3DBrowser::~Cef3DBrowser()
	{

	}

	void Cef3DBrowser::SetRootWindow(RootWindow* Wnd)
	{
		DCHECK(Wnd);
		AssociatedWindow = Wnd;
	}
}
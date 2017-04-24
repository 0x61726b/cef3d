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
	void Cef3DBrowser::LoadURL(const std::string & url)
	{
		Url = url;
		AssociatedBrowser->GetMainFrame()->LoadURL(Url);
	}

	void Cef3DBrowser::Back()
	{
		AssociatedBrowser->GoBack();
	}

	void Cef3DBrowser::Forward()
	{
		AssociatedBrowser->GoForward();
	}

	void Cef3DBrowser::Stop()
	{
		AssociatedBrowser->StopLoad();
	}

	void Cef3DBrowser::Reload()
	{
		AssociatedBrowser->Reload();
	}

	void Cef3DBrowser::ReloadIgnoreCache()
	{
		AssociatedBrowser->ReloadIgnoreCache();
	}

	void Cef3DBrowser::Focus()
	{
		AssociatedBrowser->GetHost()->SetFocus(true);
	}

	void Cef3DBrowser::Blur()
	{
		AssociatedBrowser->GetHost()->SetFocus(false);
	}

	void Cef3DBrowser::PauseRendering()
	{
		
	}

	void Cef3DBrowser::ResumeRendering()
	{
	}

	void Cef3DBrowser::Zoom()
	{
		AssociatedBrowser->GetHost()->SetZoomLevel(20);
	}

	void Cef3DBrowser::ZoomOut()
	{
		AssociatedBrowser->GetHost()->SetZoomLevel(-20);
	}

	void Cef3DBrowser::SetZoom(int Percent)
	{
		AssociatedBrowser->GetHost()->SetZoomLevel(Percent);
	}

	void Cef3DBrowser::SetWidth(int W)
	{
		
	}

	void Cef3DBrowser::SetHeight(int H)
	{
	}

	void Cef3DBrowser::Undo()
	{
		AssociatedBrowser->GetFocusedFrame()->Undo();
	}

	void Cef3DBrowser::Redo()
	{
		AssociatedBrowser->GetFocusedFrame()->Redo();
	}

	void Cef3DBrowser::Cut()
	{
		AssociatedBrowser->GetFocusedFrame()->Cut();
	}

	void Cef3DBrowser::Copy()
	{
		AssociatedBrowser->GetFocusedFrame()->Copy();
	}

	void Cef3DBrowser::Paste()
	{
		AssociatedBrowser->GetFocusedFrame()->Paste();
	}

	void Cef3DBrowser::SelectAll()
	{
		AssociatedBrowser->GetFocusedFrame()->SelectAll();
	}

	double Cef3DBrowser::GetZoomLevel()
	{
		return AssociatedBrowser->GetHost()->GetZoomLevel();
	}
	bool Cef3DBrowser::CanGoBack()
	{
		return AssociatedBrowser->CanGoBack();
	}

	bool Cef3DBrowser::CanGoForward()
	{
		return AssociatedBrowser->CanGoForward();
	}

	bool Cef3DBrowser::IsLoading()
	{
		return AssociatedBrowser->IsLoading();
	}

	bool Cef3DBrowser::IsCrashed()
	{
		return false;
	}

	void Cef3DBrowser::SetBrowserID(int ID)
	{
		BrowserID = ID;
	}
	void Cef3DBrowser::SetRootWindow(RootWindow * Wnd)
	{
		AssociatedWindow = Wnd;

		AssociatedBrowser = Wnd->GetBrowser();
	}
	void Cef3DBrowser::SetBrowser(CefRefPtr<CefBrowser> browser)
	{
		AssociatedBrowser = browser;
	}
}
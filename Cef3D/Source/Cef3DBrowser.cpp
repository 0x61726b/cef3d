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
		AssociatedWindow->GetBrowser()->GetMainFrame()->LoadURL(Url);
	}

	void Cef3DBrowser::Back()
	{
		AssociatedWindow->GetBrowser()->GoBack();
	}

	void Cef3DBrowser::Forward()
	{
		AssociatedWindow->GetBrowser()->GoForward();
	}

	void Cef3DBrowser::Stop()
	{
		AssociatedWindow->GetBrowser()->StopLoad();
	}

	void Cef3DBrowser::Reload()
	{
		AssociatedWindow->GetBrowser()->Reload();
	}

	void Cef3DBrowser::ReloadIgnoreCache()
	{
		AssociatedWindow->GetBrowser()->ReloadIgnoreCache();
	}

	void Cef3DBrowser::Focus()
	{
		AssociatedWindow->GetBrowser()->GetHost()->SetFocus(true);
	}

	void Cef3DBrowser::Blur()
	{
		AssociatedWindow->GetBrowser()->GetHost()->SetFocus(false);
	}

	void Cef3DBrowser::PauseRendering()
	{
		
	}

	void Cef3DBrowser::ResumeRendering()
	{
	}

	void Cef3DBrowser::Zoom()
	{
		AssociatedWindow->GetBrowser()->GetHost()->SetZoomLevel(20);
	}

	void Cef3DBrowser::ZoomOut()
	{
		AssociatedWindow->GetBrowser()->GetHost()->SetZoomLevel(-20);
	}

	void Cef3DBrowser::SetZoom(int Percent)
	{
		AssociatedWindow->GetBrowser()->GetHost()->SetZoomLevel(Percent);
	}

	void Cef3DBrowser::SetWidth(int W)
	{
		
	}

	void Cef3DBrowser::SetHeight(int H)
	{
	}

	void Cef3DBrowser::Undo()
	{
		AssociatedWindow->GetBrowser()->GetFocusedFrame()->Undo();
	}

	void Cef3DBrowser::Redo()
	{
		AssociatedWindow->GetBrowser()->GetFocusedFrame()->Redo();
	}

	void Cef3DBrowser::Cut()
	{
		AssociatedWindow->GetBrowser()->GetFocusedFrame()->Cut();
	}

	void Cef3DBrowser::Copy()
	{
		AssociatedWindow->GetBrowser()->GetFocusedFrame()->Copy();
	}

	void Cef3DBrowser::Paste()
	{
		AssociatedWindow->GetBrowser()->GetFocusedFrame()->Paste();
	}

	void Cef3DBrowser::SelectAll()
	{
		AssociatedWindow->GetBrowser()->GetFocusedFrame()->SelectAll();
	}

	double Cef3DBrowser::GetZoomLevel()
	{
		return AssociatedWindow->GetBrowser()->GetHost()->GetZoomLevel();
	}
	bool Cef3DBrowser::CanGoBack()
	{
		return AssociatedWindow->GetBrowser()->CanGoBack();
	}

	bool Cef3DBrowser::CanGoForward()
	{
		return AssociatedWindow->GetBrowser()->CanGoForward();
	}

	bool Cef3DBrowser::IsLoading()
	{
		return AssociatedWindow->GetBrowser()->IsLoading();
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
	}
}
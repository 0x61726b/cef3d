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
		Cef3DHandler::Get()->GetCefBrowser(this)->GetMainFrame()->LoadURL(Url);
	}

	void Cef3DBrowser::Back()
	{
		Cef3DHandler::Get()->GetCefBrowser(this)->GoBack();
	}

	void Cef3DBrowser::Forward()
	{
		Cef3DHandler::Get()->GetCefBrowser(this)->GoForward();
	}

	void Cef3DBrowser::Stop()
	{
		Cef3DHandler::Get()->GetCefBrowser(this)->StopLoad();
	}

	void Cef3DBrowser::Reload()
	{
		Cef3DHandler::Get()->GetCefBrowser(this)->Reload();
	}

	void Cef3DBrowser::ReloadIgnoreCache()
	{
		Cef3DHandler::Get()->GetCefBrowser(this)->ReloadIgnoreCache();
	}

	void Cef3DBrowser::Focus()
	{
		Cef3DHandler::Get()->GetCefBrowser(this)->GetHost()->SetFocus(true);
	}

	void Cef3DBrowser::Blur()
	{
		Cef3DHandler::Get()->GetCefBrowser(this)->GetHost()->SetFocus(false);
	}

	void Cef3DBrowser::PauseRendering()
	{
		
	}

	void Cef3DBrowser::ResumeRendering()
	{
	}

	void Cef3DBrowser::Zoom()
	{
		Cef3DHandler::Get()->GetCefBrowser(this)->GetHost()->SetZoomLevel(20);
	}

	void Cef3DBrowser::ZoomOut()
	{
		Cef3DHandler::Get()->GetCefBrowser(this)->GetHost()->SetZoomLevel(-20);
	}

	void Cef3DBrowser::SetZoom(int Percent)
	{
		Cef3DHandler::Get()->GetCefBrowser(this)->GetHost()->SetZoomLevel(Percent);
	}

	void Cef3DBrowser::SetWidth(int W)
	{
		
	}

	void Cef3DBrowser::SetHeight(int H)
	{
	}

	void Cef3DBrowser::Undo()
	{
		Cef3DHandler::Get()->GetCefBrowser(this)->GetFocusedFrame()->Undo();
	}

	void Cef3DBrowser::Redo()
	{
		Cef3DHandler::Get()->GetCefBrowser(this)->GetFocusedFrame()->Redo();
	}

	void Cef3DBrowser::Cut()
	{
		Cef3DHandler::Get()->GetCefBrowser(this)->GetFocusedFrame()->Cut();
	}

	void Cef3DBrowser::Copy()
	{
		Cef3DHandler::Get()->GetCefBrowser(this)->GetFocusedFrame()->Copy();
	}

	void Cef3DBrowser::Paste()
	{
		Cef3DHandler::Get()->GetCefBrowser(this)->GetFocusedFrame()->Paste();
	}

	void Cef3DBrowser::SelectAll()
	{
		Cef3DHandler::Get()->GetCefBrowser(this)->GetFocusedFrame()->SelectAll();
	}

	double Cef3DBrowser::GetZoomLevel()
	{
		return Cef3DHandler::Get()->GetCefBrowser(this)->GetHost()->GetZoomLevel();
	}
	bool Cef3DBrowser::CanGoBack()
	{
		return Cef3DHandler::Get()->GetCefBrowser(this)->CanGoBack();
	}

	bool Cef3DBrowser::CanGoForward()
	{
		return Cef3DHandler::Get()->GetCefBrowser(this)->CanGoForward();
	}

	bool Cef3DBrowser::IsLoading()
	{
		return Cef3DHandler::Get()->GetCefBrowser(this)->IsLoading();
	}

	bool Cef3DBrowser::IsCrashed()
	{
		return false;
	}

	void Cef3DBrowser::SetBrowserID(int ID)
	{
		BrowserID = ID;
	}
}
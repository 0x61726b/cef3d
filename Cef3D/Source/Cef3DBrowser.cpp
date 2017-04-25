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
	void Cef3DBrowser::LoadURL(const std::string & url)
	{
		Url = url;
		GMainContext->GetCefBrowser(BrowserID)->GetMainFrame()->LoadURL(Url);
	}

	void Cef3DBrowser::Close(bool force)
	{
		GMainContext->GetCefBrowser(BrowserID)->GetHost()->CloseBrowser(force);
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

	void Cef3DBrowser::SetBrowserID(int ID)
	{
		BrowserID = ID;
	}

	Cef3DBrowser::~Cef3DBrowser()
	{

	}
}
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

#pragma once

namespace Cef3D
{
	class CEF3D_API Cef3DBrowser
	{
	public:
		Cef3DBrowserType GetType() { return Type;  }
		ProcessHandle GetProcessHandle() { return ProcessHandle;  }
		int GetProcessID() { return ProcessID;  }
		int GetRoutingID() { return RoutingID; }
		int GetNextRoutingID() { return NextRoutingID; }


		/* */
		void LoadURL(const std::string& Url);
		void Back();
		void Forward();
		void Stop();
		void Reload();
		void ReloadIgnoreCache();
		void Focus();
		void Blur();
		void PauseRendering();
		void ResumeRendering();
		void Zoom();
		void ZoomOut();
		
		void SetZoom(int Percent);
		void SetWidth(int W);
		void SetHeight(int H);

		void Undo();
		void Redo();
		void Cut();
		void Copy();
		void Paste();
		void SelectAll();
		/* */

		double GetZoomLevel();
		int GetBrowserID() const { return BrowserID; }
		const std::string& GetURL() const { return Url; }
		const std::string& GetTitle() const { return Title;  }

		bool CanGoBack();
		bool CanGoForward();
		bool IsLoading();
		bool IsCrashed();

		bool IsTransparent() { return Transparent; }

	private:
		Cef3DBrowserType Type;
		int BrowserID;
		int ProcessID;
		int RoutingID;
		int NextRoutingID;
		ProcessHandle ProcessHandle;

		RootWindow* AssociatedWindow;

		std::string Url;
		std::string Title;

		bool Transparent;
		int Width;
		int Height;

	public:
		void SetBrowserID(int BrowserID);
		void SetRootWindow(RootWindow* Wnd);
	};
}
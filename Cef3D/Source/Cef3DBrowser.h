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
		Cef3DBrowser();

		~Cef3DBrowser();
		Cef3DBrowserType GetType() { return Type;  }
		ProcessHandle GetProcessHandle() { return ProcessHandle;  }
		int GetProcessID() { return ProcessID;  }
		int GetRoutingID() { return RoutingID; }
		int GetNextRoutingID() { return NextRoutingID; }

		/* Input Methods */
		void SendMouseClickEvent(Cef3DMouseEventType type, int x, int y, unsigned modifiers,int clickCount=1);
		void SendMouseWheelEvent(int x, int y, int deltaX, int deltaY, unsigned modifiers);
		void SendKeyEvent(Cef3DKeyEventType type,int native_key, int windows_key_code,bool isSystem,unsigned modifiers);
		/* */

		/* Resize */
		void SendResize(const Cef3DRect& newRect);
		void Invalidate();

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

		void Close(bool force);
		bool OnClose();
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

		/* Delegates */
	public:
		virtual void OnBeforeClose() {}
		virtual void OnPopupShow(bool) {}
		virtual void OnPopupSize(Cef3DRect) {}
		virtual void OnAfterCreated() { }
		virtual void OnPaint(Cef3DOsrRenderType, const std::vector<Cef3DRect>&, const void*, int, int) { };

		virtual void OnDevToolsReady() {}
		virtual Cef3DBrowser* GetBrowserForPopup() { return 0; }

	private:
		Cef3DBrowserType Type;
		int BrowserID;
		int ProcessID;
		int RoutingID;
		int NextRoutingID;
		ProcessHandle ProcessHandle;
		class RootWindow* AssociatedWindow;
		/*class RootWindow* AssociatedWindow;
		CefRefPtr<CefBrowser> AssociatedBrowser;*/

		std::string Url;
		std::string Title;

		bool Transparent;
		int Width;
		int Height;

		friend class MainContext;

	public:
		void SetBrowserID(int BrowserID);
		void SetRootWindow(RootWindow* Wnd);
		/*void SetRootWindow(RootWindow* Wnd);
		void SetBrowser(CefRefPtr<CefBrowser> browser);*/
	};
}
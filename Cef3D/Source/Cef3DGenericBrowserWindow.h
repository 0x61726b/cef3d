//----------------------------------------------------------------------------
// Cef3D
// Copyright (C) 2017 arkenthera
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// https://github.com/arkenthera/cef3d
// Cef3DOsrWindow.h
// Date: 23.04.2017
//---------------------------------------------------------------------------

#pragma once


namespace Cef3D
{
	class Cef3DBrowser;
	class Cef3DOsrBrowser;


	class Cef3DOsrBrowserWindow :
		public Cef3DHandlerDelegate,
		public base::RefCountedThreadSafe<Cef3DOsrBrowserWindow, DeleteOnMainThread>
	{
	public:
		virtual void SetDeviceScaleFactor(float device_scale_factor);
		virtual float GetDeviceScaleFactor() const;

		CefRefPtr<CefBrowser> GetBrowser() const;

		bool IsClosing() const;

	public:
		explicit Cef3DOsrBrowserWindow();
		/*friend struct base::DefaultDeleter<BrowserWindow>;*/

		void Init(const Cef3DBrowserDefinition& Def);

		// ClientHandler::Delegate methods.
		void OnBrowserCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
		void OnBrowserClosing(CefRefPtr<CefBrowser> browser) OVERRIDE;
		void OnBrowserClosed(CefRefPtr<CefBrowser> browser) OVERRIDE;
		void OnSetAddress(const std::string& url) OVERRIDE;
		void OnSetTitle(const std::string& title) OVERRIDE;
		void OnSetFullscreen(bool fullscreen) OVERRIDE;
		void OnSetLoadingState(bool isLoading,
			bool canGoBack,
			bool canGoForward) OVERRIDE;
		void OnSetDraggableRegions(
			const std::vector<CefDraggableRegion>& regions) OVERRIDE;

	public:
		void SetBrowser(CefRefPtr<CefBrowser> browser);

	private:
		Cef3DOsrBrowser* osrBrowser_;

		CefRefPtr<CefBrowser> browser_;
		CefRefPtr<Cef3DHandler> client_handler_;
		bool is_closing_;
	};
}
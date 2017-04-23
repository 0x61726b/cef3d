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

	class Cef3DGenericBrowserWindow :
		public Cef3DHandlerDelegate
	{
	public:
		class Delegate {
		public:
			// Called when the browser has been created.
			virtual void OnBrowserCreated(Cef3DBrowser* browser) = 0;

			//// Called when the BrowserWindow has been destroyed.
			//virtual void OnBrowserWindowDestroyed() = 0;

			//// Set the window URL address.
			//virtual void OnSetAddress(const std::string& url) = 0;

			//// Set the window title.
			//virtual void OnSetTitle(const std::string& title) = 0;

			//// Set fullscreen mode.
			//virtual void OnSetFullscreen(bool fullscreen) = 0;

			//// Set the loading state.
			//virtual void OnSetLoadingState(bool isLoading,
			//	bool canGoBack,
			//	bool canGoForward) = 0;

			//// Set the draggable regions.
			//virtual void OnSetDraggableRegions(
			//	const std::vector<CefDraggableRegion>& regions) = 0;

		protected:
			virtual ~Delegate() {}
		};

	public:
		virtual void SetDeviceScaleFactor(float device_scale_factor);
		virtual float GetDeviceScaleFactor() const;

		CefRefPtr<CefBrowser> GetBrowser() const;

		bool IsClosing() const;

	public:
		explicit Cef3DGenericBrowserWindow(Delegate* delegate);
		/*friend struct base::DefaultDeleter<BrowserWindow>;*/

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

		Delegate* delegate_;
		CefRefPtr<CefBrowser> browser_;
		CefRefPtr<Cef3DHandler> client_handler_;
		bool is_closing_;
	};
}
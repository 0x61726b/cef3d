//----------------------------------------------------------------------------
// Cef3D
// Copyright (C) 2017 arkenthera
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// https://github.com/arkenthera/cef3d
// Cef3DBrowserWindow.h
// Date: 26.04.2017
//---------------------------------------------------------------------------

#pragma once

namespace Cef3D
{
	// Represents a native child window hosting a single browser instance. The
	// methods of this class must be called on the main thread unless otherwise
	// indicated.
	class BrowserWindow : public Cef3DHandlerDelegate 
	{
	public:
		// This interface is implemented by the owner of the BrowserWindow. The
		// methods of this class will be called on the main thread.
		class Delegate {
		public:
			// Called when the browser has been created.
			virtual void OnBrowserCreated(CefRefPtr<CefBrowser> browser) = 0;

			// Called when the BrowserWindow has been destroyed.
			virtual void OnBrowserWindowDestroyed() = 0;

			// Set the window URL address.
			virtual void OnSetAddress(const std::string& url) = 0;

			// Set the window title.
			virtual void OnSetTitle(const std::string& title) = 0;

			// Set fullscreen mode.
			virtual void OnSetFullscreen(bool fullscreen) = 0;

			// Set the loading state.
			virtual void OnSetLoadingState(bool isLoading,
				bool canGoBack,
				bool canGoForward) = 0;

			// Set the draggable regions.
			virtual void OnSetDraggableRegions(
				const std::vector<CefDraggableRegion>& regions) = 0;

		protected:
			virtual ~Delegate() {}
		};

		// Create a new browser and native window.
		virtual void CreateBrowser(
			const Cef3DBrowserDefinition& settings,
			CefRefPtr<CefRequestContext> request_context) = 0;

		// Retrieve the configuration that will be used when creating a popup window.
		// The popup browser will initially be parented to |temp_handle| which should
		// be a pre-existing hidden window. The native window will be created later
		// after the browser has been created. This method may be called on any
		// thread.
		virtual void GetPopupConfig(CefWindowHandle temp_handle,
			CefWindowInfo& windowInfo,
			CefRefPtr<CefClient>& client,
			CefBrowserSettings& settings) = 0;

		// Show the popup window with correct parent and bounds in parent coordinates.
		virtual void ShowPopup(WindowHandle parent_handle,
			int x, int y, size_t width, size_t height) = 0;

		// Show the window.
		virtual void Show() = 0;

		// Hide the window.
		virtual void Hide() = 0;

		// Set the window bounds in parent coordinates.
		virtual void SetBounds(int x, int y, size_t width, size_t height) = 0;

		// Set focus to the window.
		virtual void SetFocus(bool focus) = 0;

		// Set the device scale factor. Only used in combination with off-screen
		// rendering.
		virtual void SetDeviceScaleFactor(float device_scale_factor);

		// Returns the device scale factor. Only used in combination with off-screen
		// rendering.
		virtual float GetDeviceScaleFactor() const;

		// Returns the window handle.
		virtual WindowHandle GetWindowHandle() const = 0;

		// Returns the browser owned by the window.
		CefRefPtr<CefBrowser> GetBrowser() const;

		// Returns true if the browser is closing.
		bool IsClosing() const;

	protected:
		// Allow deletion via scoped_ptr only.
		friend struct base::DefaultDeleter<BrowserWindow>;

		// Constructor may be called on any thread.
		// |delegate| must outlive this object.
		explicit BrowserWindow(Delegate* delegate);

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

	private:
		DISALLOW_COPY_AND_ASSIGN(BrowserWindow);
	};
}
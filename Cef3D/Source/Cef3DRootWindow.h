//----------------------------------------------------------------------------
// Cef3D
// Copyright (C) 2017 arkenthera
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// https://github.com/arkenthera/cef3d
// Cef3D.h
// Date: 13.04.2017
//---------------------------------------------------------------------------

#pragma once

#include "include/cef_menu_model_delegate.h"
#include "include/views/cef_button_delegate.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_browser_view_delegate.h"
#include "include/views/cef_label_button.h"
#include "include/views/cef_menu_button.h"
#include "include/views/cef_menu_button_delegate.h"
#include "include/views/cef_textfield.h"
#include "include/views/cef_textfield_delegate.h"
#include "include/views/cef_window.h"
#include "include/views/cef_window_delegate.h"

namespace Cef3D
{
	// Represents a top-level native window in the browser process. While references
	// to this object are thread-safe the methods must be called on the main thread
	// unless otherwise indicated.
	class CEF3D_API RootWindow :
		public base::RefCountedThreadSafe<RootWindow, DeleteOnMainThread> {
	public:
		// This interface is implemented by the owner of the RootWindow. The methods
		// of this class will be called on the main thread.
		class Delegate {
		public:
			// Called to retrieve the CefRequestContext for browser. Only called for
			// non-popup browsers. May return NULL.
			virtual CefRefPtr<CefRequestContext> GetRequestContext(
				RootWindow* root_window) = 0;

			// Called to exit the application.
			virtual void OnExit(RootWindow* root_window) = 0;

			// Called when the RootWindow has been destroyed.
			virtual void OnRootWindowDestroyed(RootWindow* root_window) = 0;

		protected:
			virtual ~Delegate() {}
		};

		// Create a new RootWindow object. This method may be called on any thread.
		// Use RootWindowManager::CreateRootWindow() or CreateRootWindowAsPopup()
		// instead of calling this method directly. |use_views| will be true if the
		// Views framework should be used.
		static scoped_refptr<RootWindow> Create(bool use_views);

		// Returns the RootWindow associated with the specified |browser_id|. Must be
		// called on the main thread.
		static scoped_refptr<RootWindow> GetForBrowser(int browser_id);

#if defined(OS_MACOSX)
		// Returns the RootWindow associated with the specified |window|. Must be
		// called on the main thread.
		static scoped_refptr<RootWindow> GetForNSWindow(NSWindow* window);
#endif

		// Initialize as a normal window. This will create and show a native window
		// hosting a single browser instance. This method may be called on any thread.
		// |delegate| must be non-NULL and outlive this object.
		// Use RootWindowManager::CreateRootWindow() instead of calling this method
		// directly.
		virtual void Init(RootWindow::Delegate* delegate, bool with_osr, const CefRect& bounds, const CefBrowserSettings& settings, const std::string& url) = 0;

		// Initialize as a popup window. This is used to attach a new native window to
		// a single browser instance that will be created later. The native window
		// will be created and shown once the browser is available. This method may be
		// called on any thread. |delegate| must be non-NULL and outlive this object.
		// Use RootWindowManager::CreateRootWindowAsPopup() instead of calling this
		// method directly.
		virtual void InitAsPopup(RootWindow::Delegate* delegate,
			bool with_osr,
			const CefPopupFeatures& popupFeatures,
			CefWindowInfo& windowInfo,
			CefRefPtr<CefClient>& client,
			CefBrowserSettings& settings) = 0;

		enum ShowMode {
			ShowNormal,
			ShowMinimized,
			ShowMaximized,
		};

		// Show the window.
		virtual void Show(ShowMode mode) = 0;

		// Hide the window.
		virtual void Hide() = 0;

		// Set the window bounds in screen coordinates.
		virtual void SetBounds(int x, int y, size_t width, size_t height) = 0;

		// Close the window. If |force| is true onunload handlers will not be
		// executed.
		virtual void Close(bool force) = 0;

		// Set the device scale factor. Only used in combination with off-screen
		// rendering.
		virtual void SetDeviceScaleFactor(float device_scale_factor) = 0;

		// Returns the device scale factor. Only used in combination with off-screen
		// rendering.
		virtual float GetDeviceScaleFactor() const = 0;

		bool IsSame(scoped_refptr<RootWindow> Wnd);

		// Returns the browser that this window contains, if any.
		virtual CefRefPtr<CefBrowser> GetBrowser() const = 0;

		// Returns the native handle for this window, if any.
		virtual WindowHandle GetWindowHandle() const = 0;

	protected:
		// Allow deletion via scoped_refptr only.
		friend struct DeleteOnMainThread;
		friend class base::RefCountedThreadSafe<RootWindow, DeleteOnMainThread>;

		virtual ~RootWindow() {}
	};
}
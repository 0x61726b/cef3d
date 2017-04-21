//----------------------------------------------------------------------------
// Cef3D
// Copyright (C) 2017 arkenthera
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// https://github.com/arkenthera/cef3d
// Cef3D.h
// Date: 21.04.2017
//---------------------------------------------------------------------------

#pragma once

#include "include/base/cef_scoped_ptr.h"
#include "include/cef_command_line.h"

namespace Cef3D
{
	// Used to create/manage RootWindow instances. The methods of this class can be
	// called from any browser process thread unless otherwise indicated.
	class CEF3D_API RootWindowManager : public RootWindow::Delegate {
	public:
		// If |terminate_when_all_windows_closed| is true quit the main message loop
		// after all windows have closed.
		explicit RootWindowManager(bool terminate_when_all_windows_closed);

		// Create a new top-level native window that loads |url|.
		// If |with_controls| is true the window will show controls.
		// If |with_osr| is true the window will use off-screen rendering.
		// If |bounds| is empty the default window size and location will be used.
		// This method can be called from anywhere to create a new top-level window.
		scoped_refptr<RootWindow> CreateRootWindow(
			bool with_controls,
			bool with_osr,
			const CefRect& bounds,
			const std::string& url);

		// Create a new native popup window.
		// If |with_controls| is true the window will show controls.
		// If |with_osr| is true the window will use off-screen rendering.
		// This method is called from ClientHandler::CreatePopupWindow() to
		// create a new popup or DevTools window.
		scoped_refptr<RootWindow> CreateRootWindowAsPopup(
			bool with_controls,
			bool with_osr,
			const CefPopupFeatures& popupFeatures,
			CefWindowInfo& windowInfo,
			CefRefPtr<CefClient>& client,
			CefBrowserSettings& settings);

		// Returns the RootWindow associated with the specified browser ID. Must be
		// called on the main thread.
		scoped_refptr<RootWindow> GetWindowForBrowser(int browser_id);

		// Close all existing windows. If |force| is true onunload handlers will not
		// be executed.
		void CloseAllWindows(bool force);

	private:
		// Allow deletion via scoped_ptr only.
		friend struct base::DefaultDeleter<RootWindowManager>;

		~RootWindowManager();

		void OnRootWindowCreated(scoped_refptr<RootWindow> root_window);

		// RootWindow::Delegate methods.
		CefRefPtr<CefRequestContext> GetRequestContext(
			RootWindow* root_window) OVERRIDE;
		CefRefPtr<CefImage> GetDefaultWindowIcon() OVERRIDE;
		void OnTest(RootWindow* root_window, int test_id) OVERRIDE;
		void OnExit(RootWindow* root_window) OVERRIDE;
		void OnRootWindowDestroyed(RootWindow* root_window) OVERRIDE;

		const bool terminate_when_all_windows_closed_;
		bool request_context_per_browser_;
		bool request_context_shared_cache_;

		// Existing root windows. Only accessed on the main thread.
		typedef std::set<scoped_refptr<RootWindow> > RootWindowSet;
		RootWindowSet root_windows_;

		// Singleton window used as the temporary parent for popup browsers.
		Cef3DTempWindowWin temp_window_;

		CefRefPtr<CefRequestContext> shared_request_context_;
		CefRefPtr<CefImage> default_window_icon_;

		DISALLOW_COPY_AND_ASSIGN(RootWindowManager);
	};
}